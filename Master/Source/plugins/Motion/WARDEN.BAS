interval = 1000
VR(%1) = 10000

While True
    If VR(%1) < 0 Then
        Stop "QUEUE"
        Dim m As Integer
        For axisid = 0 to %2
            m = MType Axis(axisid)
            If m <> 20 and m <> 22 and m <> 25 Then
                Cancel(2) Axis(axisid)
            Endif
        Next axisid
        For axisid = 0 to %2
            m = MType Axis(axisid)
            If m <> 20 and m <> 22 and m <> 25 Then
                Wait Idle Axis(axisid)
            Endif
        Next axisid
        RapidStop
        Halt
    Endif
    Wa(interval)
    VR(%1) = VR(%1) - interval
Wend
