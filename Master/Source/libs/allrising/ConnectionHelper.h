#pragma once

#include <functional>
#include <QCheckBox>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QLineEdit>
#include <QObject>
#include <QSpinBox>

/*!
 * ConnectionHelper 是一个协助完成控件与数据Gadget之间相互同步的模板类.
 *
 * 典型用例:
 * 当有一个属性较多的数据类 (例如 class CoreConfig) 类, 由一个编辑器 (例如 class CoreConfigEditor)
 * 来编辑时, 通常编辑器会有一个属性:
 * \code
 * class CoreConfig
 * {
 *     Q_GADGET
 *     Q_PROPERTY(
 *
 * class CoreConfigEditor : public QWidget
 * {
 *     Q_OBJECT
 *     Q_PROPERTY(CoreConfig config READ config WRITE setConfig)
 * public:
 *     ...
 * }
 * \endcode
 * CoreConfig 中的每个属性都有访问器(getter)和突变器(setter), ConnectionHelper 可以将
 * editor 中的控件与 editor.config 中的各个属性进行双向绑定:
 *  控件(control) => config:
 *      1. control 抛出信号signal, 由 ConnectionHelper 监听
 *      2. ConnectionHelper 取出 editor.config
 *      3. ConnectionHelper 对取出的 editor.config 进行更改: config.setSomeValue(newValue)
 *      4. ConnectionHelper 将更改后的config写入editor: editor.setConfig(newConfig)
 *
 *  config => control:
 *      1. 外界调用 setConfig
 *      2. setConfig 调用 ConnectionHelper.setControls
 *      3. setControls 读取 config 中各个属性, 分别调用各个控件的 setter 槽函数(control.slot)
 *
 * 为了达成以上的功能, 需要在构造时给 connection 提供以下信息:
 *  编辑器(editor)的指针
 *  从 editor 中获取 gadget 的方法 (EditorGetter)
 *  向 editor 中写入 gadget 的方法 (EditorSetter)
 *
 * 以及, 对于每个属性, 需要使用 addHandler 提供以下信息:
 *  控件(control)的指针
 *  control 的值改变时, 抛出的信号(signal)的指针
 *  向 control 中设置值的槽函数(slot)的指针
 *  从 gadget 中读取该属性的访问器(Getter)
 *  向 gadget 中写入该属性的突变器(Setter)
 * 如果 gadget 该属性没有 Getter 和 Setter, 也可以使用数据成员的指针 (Field)
 *
 * 对于常用控件, 例如SpinBox/DoubleSpinBox/ComboBox/Slider/CheckBox等控件, 内置了默
 * 认的 signal 和 slot, 可以使用 add(), 只需提供 Getter + Setter / Field 即可.
 *
 * 注意:
 *  一旦绑定完成, 不可以在析构ConnectionHelper之前析构已绑定的控件, 否则会导致野指针.
 */
template <typename Editor, typename GadgetType>
class ConnectionHelper
{
public:
    using Callback = std::function<void(const GadgetType &value)>;              /*!< 刷新控件的回调函数 */
    using EditorSetter = void (Editor::*)(const GadgetType &);                  /*!< 从Editor中读取Gadget的访问器类型 */
    using EditorGetter = GadgetType (Editor::*)() const;                        /*!< 向Editor写入Gadget的突变器类型 */

    template <typename T>
    using GadgetSetter = void (GadgetType::*)(T);
    template <typename T>
    using GadgetGetter = T (GadgetType::*)() const;
    template <typename T>
    using GadgetField = T GadgetType::*;

    ConnectionHelper(Editor *editor, EditorGetter getter, EditorSetter setter) :
        obj(editor),
        objGetter(getter),
        objSetter(setter)
    {}

    template <typename Control, typename T, typename Setter, typename Signal>
    void addHandler(Control *control, Signal slot, Signal signal, GadgetGetter<T> getter, Setter setter)
    {
        addSignal<T>(control, [setter](GadgetType &gadget, T value) -> void {
            (gadget.*setter)(value);
        }, signal);
        addSlot<T>(control, [getter](const GadgetType &gadget) {
            return (gadget.*getter)();
        } , slot);
    }

    template <typename Control, typename T, typename Signal>
    void addHandler(Control *control, Signal slot, Signal signal, GadgetField<T> field)
    {
        addSignal<T>(control, [field](GadgetType &gadget, T value) {
            (gadget.*field) = value;
        } , signal);
        addSlot<T>(control, [field](const GadgetType &gadget) {
            return (gadget.*field);
        } , slot);
    }

    /*! 监听 control 的 signal, 将更新的值用 gadget.setter 写入. */
    template <typename ValueType, typename Control, typename Func, typename Signal>
    void addSignal(Control *control, Func func, Signal signal)
    {
        using SignalType = typename QtPrivate::FunctionPointer<Signal>::Arguments::Car;

        QObject::connect(control, signal, [=](SignalType value) {
            if (isRefreshingControls) {
                return;
            }
            GadgetType c = (obj->*objGetter)();                                 /*!< 读取旧的gadget */
            func(c, ValueType(value));                                          /*!< 使用func更改它 */
            (obj->*objSetter)(c);                                               /*!< 将其写回 */
        });
    }

    /*! 注册 control 的槽函数 slot, 当 gadget 改变时, 用 getter 获取其中的一个属性,
     * 调用 control->slot 更新 control */
    template <typename ValueType, typename Control, typename Func, typename Slot>
    void addSlot(Control *control, Func func, Slot slot)
    {
        callbacks << [=](const GadgetType &c) {
            (control->*slot)(func(c));
        };
    }

    /*!
     * \brief add 将一个控件连接到当前 gadget 中: 当 gadget 中的属性改变时, 自动更新
     * 控件; 当控件被改变时, 自动更新 gadget 的值.
     * 用法:
     *  add(control, getter, setter);
     *      // add(spinBox, &Gadget::count, &Gadget::setCount);
     *  add(control, field);
     *      // add(lineEdit, &Gadget::name);
     */
    void add(QComboBox *box, GadgetGetter<QString> getter, GadgetSetter<QString> setter)
    {
        addHandler(box, &QComboBox::setCurrentText, &QComboBox::currentTextChanged, getter, setter);
    }

    void add(QComboBox *box, GadgetField<QString> field)
    {
        addHandler(box, &QComboBox::setCurrentText, &QComboBox::currentTextChanged, field);
    }

    template <typename ... Args>
    void add(QComboBox *box, Args ... args)
    {
        addHandler(box, &QComboBox::setCurrentIndex, &QComboBox::currentIndexChanged, args ...);
    }

    template <typename ... Args>
    void add(QDoubleSpinBox *box, Args ... args)
    {
        addHandler(box, &QDoubleSpinBox::setValue, &QDoubleSpinBox::valueChanged, args ...);
    }

    template <typename ... Args>
    void add(QSpinBox *box, Args ... args)
    {
        addHandler(box, &QSpinBox::setValue, &QSpinBox::valueChanged, args ...);
    }

    template <typename ... Args>
    void add(QAbstractSlider *slider, Args ... args)
    {
        addHandler(slider, &QAbstractSlider::setValue, &QAbstractSlider::valueChanged, args ...);
    }

    template <typename ... Args>
    void add(QCheckBox *box, Args ... args)
    {
        addHandler(box, &QCheckBox::setChecked, &QCheckBox::toggled, args ...);
    }

    template <typename ... Args>
    void add(QLineEdit *edit, Args ... args)
    {
        addHandler(edit, &QLineEdit::setText, &QLineEdit::textChanged, args ...);
    }

    /*! 使用新的 Gadget 值刷新所有 control */
    void setControls(const GadgetType &c)
    {
        isRefreshingControls = true;
        for (const auto &callback: callbacks) {
            callback(c);
        }
        isRefreshingControls = false;
    }

    /*! 使用指定的 gadget.func(...) 更改 gadget */
    template <typename Func, typename ...T>
    void set(Func func, T ...args)
    {
        GadgetType c = (obj->*objGetter)();                                     /*!< 读取旧的gadget */
        (c.*func)(args...);                                                     /*!< 使用func更改它 */
        (obj->*objSetter)(c);                                                   /*!< 将其写回 */
    }

    /*! 使用指定的 gadget.func(enum) 更改 gadget.
     *  由于部分控件的值类型是 int, gadget 中的类型是 enum, 这个模板重载进行了类型转换 */
    template <typename Func>
    void set(Func func, int value)
    {
        /* Func 的第一个参数类型 */
        using ValueType = typename QtPrivate::FunctionPointer<Func>::Arguments::Car;
        /* 对 value 进行类型转换, 再调用前一个重载 */
        set<Func, ValueType>(func, ValueType(value));
    }

    /*! 更改 gadget 的指定数据成员 field. 等价于 gadget.field = value */
    template <typename F, typename T>
    void setField(GadgetField<F> field, T value)
    {
        GadgetType c = (obj->*objGetter)();
        c.*field = value;
        (obj->*objSetter)(c);
    }

private:
    Editor *obj = nullptr;
    EditorGetter objGetter = nullptr;
    EditorSetter objSetter = nullptr;
    QList<Callback> callbacks;
    bool isRefreshingControls = false;
};
