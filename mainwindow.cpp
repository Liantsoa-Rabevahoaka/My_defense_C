#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->username->setPlaceholderText("Enter Your Username");
    ui->password->setPlaceholderText("Enter Your Password");
    ui->passwordLogin->setPlaceholderText("Enter Your Password");
    ui->email->setPlaceholderText("Enter Your Email");
    ui->emailLogin->setPlaceholderText("Enter Your Email");
    ui->phone->setPlaceholderText("Enter Your Phone");
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_registerButton_clicked()
{
    database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName("qt5register");

    if(database.open()){

        //Retreive Data from input fields
        QString username = ui->username->text();
        QString password = ui->password->text();
        QString email = ui->email->text();
        QString phone = ui->phone->text();

        //Run our insert query
        QSqlQuery qry;
        qry.prepare("INSER INTO users (username, password, email, phone)"
                    "VALUES (:username, :password, :email, :phone");

        qry.bindValue(":username", username);
        qry.bindValue(":password", password);
        qry.bindValue(":email", email);
        qry.bindValue(":phone", phone);

        if(qry.exec()){

            QMessageBox::information(this, "Inserted", "Data inserted Succesfully");

        }else{

            QMessageBox::information(this, "Not Inserted", "Data is not inserted");

        }

    }else{

        QMessageBox::information(this, "Not connected", "Database is not connected");

    }
}


void MainWindow::on_loginButton_clicked()
{
    QSqlDatabase db;
    db = QSqlDatabase::addDatabase("QSQLITE", "MyConnect");
    db.setDatabaseName("qt5register");

    QString email = ui->emailLogin->text();
    QString password = ui->passwordLogin->text();

    if(db.open()){

        QMessageBox::information(this, "Connected", "Database is connected");

        //Creating My Queris
        QSqlQuery query(QSqlDatabase::database("MyConnect"));
        query.prepare(QString("SELECT * FROM users WHERE email = :email AND password = :password"));

        query.bindValue("email", email);
        query.bindValue("password", password);

        if(!query.exec()){

            QMessageBox::information(this, "Failed", "Query Failed To Execute");

        }else{

            while(query.next()){

                QString emailFromDB = query.value(1).toString();
                QString passwordFromDB = query.value(2).toString();

                if(emailFromDB == email && passwordFromDB == password){

                    QMessageBox::information(this, "Success", "Login Success");

                    MyDialog dialog;
                    dialog.setModal(true);
                    dialog.exec();

                }else if (emailFromDB == email && passwordFromDB != password ) {

                    QMessageBox::information(this, "Failed", "Password invalid");

                }else if (emailFromDB != email && passwordFromDB == password) {

                    QMessageBox::information(this, "Failed", "Email invalid");

                }else{

                    QMessageBox::information(this, "Failed", "Email and password invalid");

                }
            }
        }

    }else{

        QMessageBox::information(this, "Not connected", "Database is not connected");

    }
}

