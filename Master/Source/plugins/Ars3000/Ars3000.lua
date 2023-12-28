-- Utils
function print(...)
    for index = 1, select("#", ...) do
        if index > 1 then
            SocketPrint("\t")
        end
        SocketPrint(tostring(select(index, ...)))
    end
    SocketPrint("\n")
end

function printf(msg, ...)
    print(msg:format(...))
end

function math.bound(min, value, max)
    assert(min <= max, "Bad args for math.bound: invalid min and max")
    return math.max(min, math.min(max, value))
end

function ReadFile(path)
    local f, err = io.open(path, "r")
    if f then
        local str = f:read("*all")
        f:close()
        return str
    end
    return "", err
end

function WriteTable(write, table, indent)
    if not write then
        return
    end
    local indent = indent or ""
    write(indent, "{\n")
    for key, value in pairs(table) do
        write(indent)
        if type(key) == "number" then
            write("[", key, "]")
        elseif type(key) == "string" then
--            write("[", string.format("%q", key), "]")
            write(key)
        end
        write(" = ")
        if type(value) == "number" then
            write(value)
        elseif type(value) == "string" then
            write(string.format("%q", value))
        elseif type(value) == "table" then
            WriteTable(file, value, indent .. "\t")
        elseif type(value) == "boolean" then
            write(tostring(value))
        else
            write("nil")
        end
        write(",\n")
    end
    write(indent, "}")
end

local loadstring = loadstring or load
local FPGA_FREQUENCY = 100
local FPGA_CYCLE_NANOSEC = 1000 / FPGA_FREQUENCY
local AD_FREQUENCY = 400
local DA_FREQUENCY = 80
local MAX_WAVEFORM_AMP = 1024
local MAX_INT16 = 65535
local SAMPLE_SPAN = 512
local TCG_SPAN = 1024
local SAMPLE_BUFFER_SIZE = 16384
local GAIN_COMPENSATE = "Band%dChannel%dGain%d"
local RECTIFIER_ZERO = "Channel%dRectifierZero"
local hardwareName = ReadFile("/proc/device-tree/model")
local hardwareVersion = 2
local instrumentInfo = {}
local variables = {}
local prevSetInstrument
local Ars3000Config = {}
local models = {
    [0x5010] = "ARS501",
}

function OnClientConnected(ip)
    printf("Client %s connected.", ip)
end

function OnClientDisconnected(ip)
    printf("Client %s disconnected.", ip)
end

function OnLoad()
    local version, revision = GetVersion()
    instrumentInfo = MakeInstrumentInfo()
    printf("Welcome to ARS3000 version %s (%s) serving at %s", version, revision, GetHostIP())
    if type(_OS) == "string" then
        printf("OS is %s", _OS)
    end
    printf("Your IP is %s", GetClientIP())
    if hardwareName then
        printf("Hardware: \t%s", hardwareName)
        if hardwareName == "ARS500P" then
            hardwareVersion = tonumber(ReadFile("/proc/device-tree/hardware-version") or 0) or 3
        else
            hardwareVersion = tonumber(hardwareName:match("Allrising ARS500 V(%d+)")) or hardwareVersion
        end
    end
    printf("DeviceID:\t%s", GetDeviceID() or "Unknown")
    printf("Channels:\t%d", GetNumChannels())

    SetMemoryInt16(0x000002, 0)
    SetMemoryInt16(0x000002, 1)
    SetMemoryInt16(0x000002, 0)
    SetMemoryInt16(0x000022, 3)     -- 工作模式

    SetMemoryInt16(0x00002C, 0x7F)  -- 编码器使能
    SetMemoryInt16(0x00002E, 0)     -- 编码器复位
    SetMemoryInt16(0x000058, 0xFF)
    SetMemoryInt16(0x00005A, 0xFF)
    SetMemoryInt16(0x00005C, 1)

    local version, revision = GetVersion()
    variables.Version = version
    variables.Revision = revision
    variables.Os = _OS
    variables.Model = string.format("%x", GetMemoryInt16(0x2A))
    variables.FpgaRevision = string.format("%04x", GetMemoryInt16(0x24))
    variables.Func1 = string.format("%x", GetMemoryInt16(0x26))
    variables.Func2 = string.format("%x", GetMemoryInt16(0x26))

    Ars3000 = {
        {{}, {}, {}, {}},
        {{}, {}, {}, {}},
    }
    LoadConfig()
    ResolveConfig()
--    Ars3000:Set("ProbeMultiplex", 0)

    ReportConfig()
    ReportInstrumentInfo()
--    c()
end

function ResetEncoders()
    SetMemoryInt16(0x2E, 1)
    Sleep(1)
    SetMemoryInt16(0x2E, 0)
end

local memoryCache = {}
function SetMemory(addr, value)
    if memoryCache[addr] ~= value then
        memoryCache[addr] = value
        SetMemoryInt16(addr, value)
    end
end

local instrumentKeys = {
    SysReset        = {address = 0x000000, readOnly = false, },
    Relays          = {address = 0x000002, readOnly = false, },
    WorkMode        = {address = 0x000022, readOnly = false, },
    Version         = {address = 0x000024, readOnly = true,  },
    SoftwareFunc1   = {address = 0x000026, readOnly = true, },
    SoftwareFunc2   = {address = 0x000028, readOnly = true, },
    Model           = {address = 0x00002A, readOnly = true, },
    EncoderEnable   = {address = 0x00002C, readOnly = false, },
    EncoderReset    = {address = 0x00002E, readOnly = false, },
    LED             = {address = 0x000050, readOnly = false, },
    ProbeMultiplex  = {address = 0x000052, readOnly = false, },
    PRF             = {address = 0x040002},
    Voltage         = {address = 0x000062},
}
local channelOffsets = {
    0x040000,
    0x080000,
}
local channelKeys = {
    ChannelReset        = {address = 0x000000},
    PulseWidth          = {address = 0x000004},
    AlterSource         = {address = 0x00000A},
    ProbeBand           = {address = 0x000010},
    RectifierMode       = {address = 0x020000},
    RectifierZero       = {address = 0x020002},
    Suppress            = {address = 0x02000E},
    Zero                = {address = 0x020010},
    TcgEnable           = {address = 0x020012},
    TcgInterval         = {address = 0x020090},
    TcgRam              = {address = 0x021000},
    SampleInterval      = {address = 0x020044},
    SampleAverage       = {address = 0x020046},
    Delay               = {address = 0x020048},
    Range               = {address = 0x020800},
    PreAmplify          = {address = 0x000006},
    Amplify1            = {address = 0x020006},
    Amplify2            = {address = 0x020008},
    SurfaceTracing      = {address = 0x020092},
    SurfaceOffset       = {address = 0x020094},
    SurfaceThreshold    = {address = 0x020096},
    SurfaceStart        = {address = 0x020098},
    SurfaceEnd          = {address = 0x02009A},
}
local gateOffsets = {
    0x020014 + 0x00,
    0x020014 + 0x0C,
    0x020014 + 0x18,
    0x020014 + 0x24,
}
local gateKeys = {
    Enable          = {address = 0x00},
    SyncMode        = {address = 0x02},
    SyncThreshold   = {address = 0x04},
    Start           = {address = 0x06},
    Width           = {address = 0x08},
    Amp             = {address = 0x0A},
}
local voltageTable = {
    [50] = 46,
    [100] = 107,
    [150] = 161,
    [200] = 194,
    [250] = 216,
    [300] = 233,
    [350] = 246,
    [400] = 255,
}

function Set(offset, key, value)
    SetMemory(offset + key.address, value or 0)
end

function SetInstrument(config)
    Set(0, instrumentKeys.PRF, FPGA_FREQUENCY * 1.0E6 / 256 / (config.PRF or 100))
    Set(0, instrumentKeys.ProbeMultiplex, config.ProbeMultiplex)
--    Set(0, instrumentKeys.TransmitVoltage, TransmitVoltage)

    SetRelays(config)
    SetWorkMode(config)
    for index, channelOffset in ipairs(channelOffsets) do
        SetChannel(channelOffset, config, index)
    end
end

function SetTimebase(offset, config)
    local range = config.Range or 1
    local delay = math.max(0, config.Delay or 0)
    local rangeInCycles = range * AD_FREQUENCY

    local sampleSize = math.floor(rangeInCycles / SAMPLE_BUFFER_SIZE)
    Set(offset, channelKeys.SampleInterval, math.max(0, sampleSize))

    local delayInCycles = math.bound(0, delay * FPGA_FREQUENCY, MAX_INT16)
    Set(offset, channelKeys.Delay, delayInCycles)
    Set(offset, channelKeys.Zero, 0)

    local rangeInSamples = rangeInCycles / (sampleSize + 1)
    local prevOffsetInSamples = 0
    for index = 0, SAMPLE_SPAN - 1 do
        local offsetInSamples = math.ceil(rangeInSamples / SAMPLE_SPAN * (index + 1))
        Set(offset + index * 2, channelKeys.Range, math.max(0, offsetInSamples - prevOffsetInSamples - 1))
        prevOffsetInSamples = offsetInSamples
    end
    for index, gateOffset in ipairs(gateOffsets) do
        local gate = config[index]
        if gate then
            SetGateTimebase(offset + gateOffset, gate, index, delay, sampleSize + 1)
            Set(offset + gateOffset, gateKeys.Amp, (gate.Amp or 0) / 100 * 1023)
            Set(offset + gateOffset, gateKeys.Enable, gate.Enable)
        end
    end

    SetTcg(offset, config.TCG, range)
end

function SetGateTimebase(offset, config, index, delay, sampleInterval)
    local start = config.Start or 0
    local width = config.Width or 0
    local startOffset = start
    local widthOffset = width
    if index == 1 then
        startOffset = math.max(0, start - delay)
        widthOffset = start + width - delay - startOffset
    end
    local startSamples = math.bound(0, startOffset * AD_FREQUENCY / sampleInterval, 16383)
    local widthSamples = math.bound(0, widthOffset * AD_FREQUENCY / sampleInterval, 16383 - startSamples)
    Set(offset, gateKeys.Start, startSamples)
    Set(offset, gateKeys.Width, widthSamples)
    -- printf("SetTimes: start %d width %d, sampleInterval %d", startSamples, widthSamples, sampleInterval)
end

function SetTcg(offset, t, range)
    local t = t or {}
    local cycles = range * DA_FREQUENCY
    local sampleInterval = math.ceil(cycles / TCG_SPAN)
    Set(offset, channelKeys.TcgInterval, sampleInterval)
    local interval = sampleInterval / DA_FREQUENCY

    local tcgIndex = 1
    local first = t[tcgIndex] or {Time = 0, Gain = 0}
    local second = first
    for index = 0, TCG_SPAN - 1 do
        local time = index * interval
        while time > second.Time do
            tcgIndex = tcgIndex + 1
            first = second
            second = t[tcgIndex] or {Time = first.Time + range, Gain = first.Gain}
        end

        local scale = (time - first.Time) / (second.Time - first.Time)
        local gain = (second.Gain - first.Gain) * scale + first.Gain
        if math.abs(second.Time - first.Time) <= 1e-6 then
            gain = first.Gain
        end
        Set(offset + index * 2, channelKeys.TcgRam, math.bound(0, gain / 40 * 1023, 1023))
    end
end

function SetWorkMode(config)
    if config[2].AlterSource == 1 then
        Set(0, instrumentKeys.WorkMode, 6)
    else
        local mode = 0
        for index, channel in ipairs(config) do
            if channel.PulserEnable ~= 0 then
                mode = mode + 2 ^ (index - 1)
            end
        end
        Set(0, instrumentKeys.WorkMode, mode)
    end
    Set(channelOffsets[1], channelKeys.AlterSource, 0)
    Set(channelOffsets[2], channelKeys.AlterSource, config[2].AlterSource)
end

function SetRelays(config)
    local v = 0
    local voltage = tonumber(config.TransmitVoltage or 0)
    if voltage == 0 or voltage == 1 then -- Legacy voltage settings, based on relays.
        voltage = (voltage == 1) and 400 or 100
    end
    if hardwareVersion <= 2 then
        v = v + ((voltage == 400) and 1 or 0) * 2 ^ 0
    else
        Set(0, instrumentKeys.Voltage, voltageTable[voltage] or voltageTable[100])
    end
    v = v + (config[1].Impedance        or 0) * 2 ^ 1
    v = v + (config[1].TransmitEnergy   or 0) * 2 ^ 2
    v = v + (config[1].TransmitMode     or 0) * 2 ^ 3
    v = v + (config[2].Impedance        or 0) * 2 ^ 4
    v = v + (config[2].TransmitEnergy   or 0) * 2 ^ 5
    v = v + (config[2].TransmitMode     or 0) * 2 ^ 6
    v = v + (config[1].PulseType        or 0) * 2 ^ 7
    v = v + (config[2].PulseType        or 0) * 2 ^ 8
    Set(0, instrumentKeys.Relays, v)
end

function SetChannel(offset, config, channelIndex)
    local c = config[channelIndex]
    if c.AlterSource == 1 then
        c = config[1]
    end
    Set(offset, channelKeys.Suppress, (c.Suppress or 0) / 100 * MAX_WAVEFORM_AMP)
    Set(offset, channelKeys.PulseWidth, (c.PulseWidth or 50) / FPGA_CYCLE_NANOSEC)
    Set(offset, channelKeys.ProbeBand, c.ProbeBand)
    Set(offset, channelKeys.RectifierMode, c.RectifierMode)
    Set(offset, channelKeys.RectifierZero, c.RectifierZero or 511)
    Set(offset, channelKeys.TcgEnable, c.TcgEnable)
    Set(offset, channelKeys.SampleAverage, c.SampleAverage)
    Set(offset, channelKeys.SurfaceTracing, c.SurfaceTracing)
    Set(offset, channelKeys.SurfaceOffset, (c.SurfaceOffset or 0) * FPGA_FREQUENCY)
    Set(offset, channelKeys.SurfaceThreshold, (c.SurfaceThreshold or 0) / 100 * 1024)
    Set(offset, channelKeys.SurfaceStart, (c.SurfaceStart or 0) * FPGA_FREQUENCY)
    Set(offset, channelKeys.SurfaceEnd, (c.SurfaceEnd or 0) * FPGA_FREQUENCY)

    SetMemoryInt16(offset + 0x000008, 1)
--    SetMemoryInt16(offset + 0x00000A, 0)
    SetMemoryInt16(offset + 0x00000C, 1)
    SetMemoryInt16(offset + 0x00000E, 1)
--    SetMemoryInt16(offset + 0x000010, 5)

    SetGain(offset, config, channelIndex)
    SetTimebase(offset, c)
end

function SetGainManual(offset, PreAmplify, Amplify1, Amplify2)
    Set(offset, channelKeys.PreAmplify, PreAmplify)
    Set(offset, channelKeys.Amplify1, Amplify1)
    Set(offset, channelKeys.Amplify2, Amplify2)
end

function MaybeSwapGains(PreAmplify, Amplify1, Amplify2)
    local info = instrumentInfo
    if info.FpgaRevisionInt <= 0x521 and hardwareVersion >= 3 then
        return PreAmplify, Amplify2, Amplify1
    end
    return PreAmplify, Amplify1, Amplify2
end

function SetGain(offset, config, channelIndex)
    local self = config[channelIndex]
    local gain = math.bound(0, self.Gain or 0, 110)
    if self.ManualGain == 1 then
        return SetGainManual(offset, self.PreAmplify, self.Amplify1, self.Amplify2)
    end
    if channelIndex == 2 and config[2].AlterSource == 1 then
        local master = config[1]
        local baseGain = math.bound(0, master.Gain or 0, 110)
        local gain = math.bound(0, baseGain + master.GainRef or 0, 110)
        -- printf("Setting ref gain for channel 2: base %f, gain %f", baseGain, gain)
        local probeBand = config[1].ProbeBand or 0
        if hardwareVersion >= 3 then
            SetGainManual(offset, MaybeSwapGains(CalcBaseGain(gain, Ars3000Config.gains[probeBand + 1][1] or 0)))
        else
            SetGainManual(offset, MaybeSwapGains(1, CalcAlterGain(config[1].Gain or 0, config[1].GainRef or 0, config.gains[probeBand + 1][1] or 0)))
        end
    else
        local probeBand = self.ProbeBand or 0
        SetGainManual(offset, MaybeSwapGains(CalcBaseGain(self.Gain or 0, Ars3000Config.gains[probeBand + 1][channelIndex] or 0)))
    end
end

function CalcBaseGain(gain, gainCompensate)
    if gain < 32 then
        return 1, 4 / 40 * 1023, (gain + 4) / 40 * 1023
    elseif gain < 64 then
        return 2, 4 / 40 * 1023, (gain + gainCompensate[1] + 4 - 32) / 40 * 1023
    elseif gain < (84 - gainCompensate[2]) then
        return 4, 4 / 40 * 1023, (gain + gainCompensate[2] + 4 - 48) / 40 * 1023
    else
        return 4, (gain + 4 - (84 - gainCompensate[2])) / 40 * 1023, 1023
    end
end

function CalcAlterGain(gainBase, gain, gainCompensate)
    gain = gainBase + gain
    if gainBase < 32 then
        if gain <= 36 then
            return 4 / 40 * 1023, (gain + 4) / 40 * 1023
        elseif gain <= 72 then
            return (gain + 4 - 36) / 40 * 1023, 1023
        else
            return 1023, 1023
        end
    elseif gainBase < 64 then
        if gain + 4 + gainCompensate[1] - 32 <= 40 then
            return 4 / 40 * 1023, (gain + 4 + 0.9 - 32) / 40 * 1023
        elseif gain + 4 + gainCompensate[1] - 32 <= 76 then
            return (gain + 4 + gainCompensate[1] - 32 - 36) / 40 * 1023, 1023
        else
            return 1023, 1023
        end
    else
        if gain + gainCompensate[2] + 4 - 48 <= 40 then
            return 4 / 40 * 1023, (gain + gainCompensate[2] + 4 - 48) / 40 * 1023
        elseif gain + gainCompensate[2] + 4 - 48 <= 76 then
            return (gain + gainCompensate[2] + 4 - 48 - 36) / 40 * 1023, 1023
        else
            return 1023, 1023
        end
    end
end

function MakeInstrumentInfo()
    local version, revision = GetVersion()
    local fpgaRevision = GetMemoryInt16(0x24)
    local info = {
        Version = version,
        Revision = revision,
        Os = _OS,
        Model = string.format("%x", GetMemoryInt16(0x2A)),
        FpgaRevision = string.format("%04x", fpgaRevision),
        FpgaRevisionInt = fpgaRevision,
        Func1 = string.format("%x", GetMemoryInt16(0x26)),
        Func2 = string.format("%x", GetMemoryInt16(0x26)),
        HardwareName = hardwareName,
    }
    if _OS == "Linux" then
        os.execute("ifconfig > /tmp/ars3000_ifconfig")
        local ifconfig = ReadFile("/tmp/ars3000_ifconfig")
        info.Ip = ifconfig:match("inet addr:([%d%.]+)")
        info.Mac = ifconfig:match("HWaddr ([%da-fA-F%:]+)")
    end
    if fpgaRevision >= 0x521 then
        info.FpgaSvnRevision = GetMemoryInt16(0x106)
        local fpgaCompileTime = GetMemoryInt32(0x108)
        info.FpgaCompileTime = os.date("%c", fpgaCompileTime)
    end

    return info
end

function ReportConfig()
    if _OS == "Linux" then
        os.execute("ifconfig > /tmp/ars3000_ifconfig")
        local ifconfig = ReadFile("/tmp/ars3000_ifconfig")
        variables.Ip = ifconfig:match("inet addr:([%d%.]+)")
        variables.Mac = ifconfig:match("HWaddr ([%da-fA-F%:]+)")
    end

    local report = {}
    for key, value in pairs(variables) do
        if type(value) == "string" then
            table.insert(report, ([["%s": "%s"]]):format(key, value))
        else
            table.insert(report, ([["%s": %s]]):format(key, value))
        end
    end
    print("REPORT_CONFIG\t{" .. table.concat(report, ",") .. "}")
end

function ReportInstrumentInfo()
    local r = {}
    local modelId = GetMemoryInt16(0x2A)

    r.model = models[modelId] or string.format("%04x", modelId)
    r.hardwareVersion = string.format("%x %x", GetMemoryInt16(0x26), GetMemoryInt16(0x28))
    r.fpgaVersion = string.format("%04x", GetMemoryInt16(0x24))
    local version, revision = GetVersion()
    r.softwareVersion = version .. " r" .. revision

    local report = {}
    for key, value in pairs(r) do
        if type(value) == "string" then
            table.insert(report, ([["%s": "%s"]]):format(key, value))
        else
            table.insert(report, ([["%s": %s]]):format(key, value))
        end
    end
    print("REPORT_INFO\t{" .. table.concat(report, ",") .. "}")
end

function TryLoadConfig(path)
    printf("Try loading config from %s", path)
    local data = ReadFile(path)
    if #data == 0 then
        return false
    end
    local func = loadstring("return " .. data)  -- Lua style config
    if #data > 0 and func then
        variables = func()
        printf("Config loaded as lua")
        return true
    end
    for band, channel, index, value in data:gmatch("(%d)[xX](%d)(%d)%s*=%s*([%-%.%d]+)") do  -- 1X11=-0.6
        band = tonumber(band)
        channel = tonumber(channel)
        index = tonumber(index)
        value = tonumber(value)
        variables[GAIN_COMPENSATE:format(band, channel, index)] = value
--        printf("Using gain ch %d - band %d - index %d: %d", channel, band, index, value)
    end
    for index, value in data:gmatch("MidValue(%d+)%s*=%s*(%d+)") do   -- MidValue1=511
        index = tonumber(index)
        value = tonumber(value)
--        printf("Using rectifier zero ch %d: %d", index, value)
        variables[RECTIFIER_ZERO:format(index)] = value
    end
    printf("Config loaded as ini")
    return true
end

function LoadConfig()
    local dir = ""
    if GetWorkingDirectory then
        dir = GetWorkingDirectory():match("^\\(.+[\\/])[^\\/]*$")
    end
    local sources = {
        "Param.ini",
        "Config.lua",
    }
    for _, fileName in ipairs(sources) do
        if TryLoadConfig(dir .. fileName) then
            return printf("Config loaded from %s%s", dir, fileName)
        end
    end
    printf("Config not found")
end

function ResolveConfig()
    local gains = {}
    for band = 1, 8 do
        local bandGains = {}
        for channel = 1, 2 do
            local channelGains = {}
            for index = 1, 2 do
                local key = GAIN_COMPENSATE:format(band, channel, index)
                local value = tonumber(variables[key]) or 0;
                variables[key] = value
                channelGains[index] = value
            end
            table.insert(bandGains, channelGains)
            Ars3000[channel].RectifierZero = variables[RECTIFIER_ZERO:format(channel)]
        end
        table.insert(gains, bandGains)
    end
    variables.Serial = variables.Serial or ""
    Ars3000Config.gains = gains
    c()
end

function SetConfig(t)
    local file = io.open("Config.lua", "w")
    if file then
        WriteTable(function (...) file:write(...) end, t)
        file:close()
        LoadConfig()
        ResolveConfig()
        ReportConfig()
        ReportInstrumentInfo()
    end
end

function ChangeIP(ip)
    local p1, p2, p3, p4 = ip:match("(%d+)%.(%d+)%.(%d+)%.(%d+)")
    if not p1 or p4 == "1" then
        return printf("Failed changing IP: invalid ip %s", ip)
    end
    local config, err = ReadFile("/etc/network/interfaces")
    if not config or #config == 0 then
        return printf("Failed changing IP: can't open config file: %s", tostring(err))
    end

    local newConfig = {}
    local removeLine = false
    for line in config:gmatch("(.-)\n") do
        local indent, command, arg = line:match("^(%s*)(%S+) (%S+)")
        if not indent then
            removeLine = false
        end
        if command == "iface" and arg == "eth0" then
            removeLine = true
        end
        if not removeLine then
            table.insert(newConfig, line)
        end
    end

    table.insert(newConfig, "iface eth0 inet static")
    table.insert(newConfig, ("\taddress %d.%d.%d.%d"):format(p1, p2, p3, p4))
    table.insert(newConfig, ("\tnetmask 255.255.255.0"))
    table.insert(newConfig, ("\tnetwork %d.%d.%d.0"):format(p1, p2, p3))
    table.insert(newConfig, ("\tgateway %d.%d.%d.1"):format(p1, p2, p3))

    print("Applying new config:")
    print(table.concat(newConfig, "\n"))

    local output, err = io.open("/etc/network/interfaces", "w")
    if not output then
        return printf("Failed changing IP: can't open output file: %s", tostring(err))
    end
    output:write(table.concat(newConfig, "\n"))
    output:close()

    CloseSocket()
    os.execute("reboot")
end

local configChanged = false
function OnDataReady()
    if configChanged then
        configChanged = false
        SetInstrument(Ars3000)
        prevSetInstrument = os.time()
    end
end

function c()
    configChanged = true
    if not prevSetInstrument or os.time() - prevSetInstrument > 2 then
        OnDataReady()
    end
end

OnLoad()
