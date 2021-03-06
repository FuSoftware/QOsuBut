#include "QTaikoBotwidget.h"

QTaikoBotWidget::QTaikoBotWidget(QString executable, QWidget *parent) : QWidget(parent)
{
    this->bot = new QTaikoBotAsync(executable);
    scan = new QLabel("Time", this);
    process = new QLabel("Time", this);
    running = new QLabel("Run State", this);
    QPushButton *pb = new QPushButton("Start",this);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(running);
    layout->addWidget(scan);
    layout->addWidget(process);
    layout->addWidget(pb);

    QHBoxLayout *pixelLayout = new QHBoxLayout;
    layout->addLayout(pixelLayout);

    for(int i=0;i<2;i++)
    {
        QLabel *l = new QLabel(this);
        this->pixels.push_back(l);
        pixelLayout->addWidget(l);
    }

    connect(pb,SIGNAL(clicked(bool)),this,SLOT(start()));

    connect(bot,SIGNAL(scanTime(int)),this,SLOT(onScan(int)));
    connect(bot,SIGNAL(processTime(int)),this,SLOT(onProcess(int)));

    connect(bot,SIGNAL(pixelRead(int,QPixmap)),this,SLOT(onPixel(int,QPixmap)));
    connect(bot,SIGNAL(running(bool)),this,SLOT(onRunning(bool)));

    this->resize(300,300);
}

void QTaikoBotWidget::onScan(int elapsed)
{
    this->scan->setText(QString("Scan : ") + QString::number(elapsed) + QString("ms"));
}

void QTaikoBotWidget::onProcess(int elapsed)
{
    this->process->setText(QString("Process :") + QString::number(elapsed) + QString("ms"));
}

void QTaikoBotWidget::onRunning(bool running)
{
    if(running)
        this->running->setText("Running");
    else
        this->running->setText("Not Running");

}

void QTaikoBotWidget::onPixel(int id, QPixmap value)
{
    this->pixels[id]->setPixmap(value.scaled(50,50));
}

void QTaikoBotWidget::start()
{
    QThread *t = new QThread(this);
    this->bot->moveToThread(t);

    //Thread Management
    connect(t, SIGNAL(started()),  this->bot, SLOT(start()));
    connect(this->bot, SIGNAL(finished()), t, SLOT(quit()));
    connect(this->bot, SIGNAL(finished()), this->bot, SLOT(deleteLater()));
    connect(t, SIGNAL(finished()), t, SLOT(deleteLater()));

    t->start();

}
