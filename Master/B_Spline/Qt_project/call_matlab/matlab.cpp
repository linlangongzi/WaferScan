#include "matlab.h"
#include "ui_matlab.h"
#include <string.h>
#include <QString>
#include <QKeyEvent>
#include <QMessageBox>

matlab::matlab(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::matlab)
{
    ui->setupUi(this);
    ui->result->append("matlab engine has not been open!");
  //  ep = engOpen("");
    flag_eng = false;    //engine打开
  //  engSetVisible(ep, true);//显示matlab引擎
    connect(ui->command,SIGNAL(editingFinished()),this,SLOT(response()));
}

matlab::~matlab()
{
    delete ui;
}
void matlab::response()
{
    if(flag_eng == true && ui->command->text()=="mclose"){//close matlab engine
        engClose(ep);
        flag_eng = false;
        ui->result->append("matlab engine is closed!");
    }
    else if(flag_eng == false && ui->command->text() == "mopen"){   //open the matlab engine
        ep = engOpen("");
        flag_eng = true;
        ui->result->append("matlab engine is open!");
    }
    else if(flag_eng == false && ui->command->text() == "close" ){  //close the app
        this->close();
    }
    else if(flag_eng == true){                       //当matlab引擎关闭时，不可操作
        ui->result->append(ui->command->text());
        //实现Qstring到char*类型的转换，实现将命令输入到matlab
        QString str = ui->command->text();
        char* ch;
        QByteArray ba = str.toLatin1();
        ch = ba.data();

        engOutputBuffer(ep, buffer, SIZE);        //buffer记录matlab的输出结果
        engEvalString(ep,ch);                     //将命令输出到matlab
        ui->result->append(buffer);               //将计算结果显示在窗口中
        if(ui->command->text()=="clear")
            ui->result->clear();
        ui->command->clear();
    }
    else{
        QMessageBox::information(this, "Info", "matlab engine is closed!", QMessageBox::Cancel);
        //ui->result->append("matlab engine is closed!");
    }
    ui->command->clear();
}

