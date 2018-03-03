#ifndef WINDOWHANDLERWORKER_H
#define WINDOWHANDLERWORKER_H

#include <QObject>
#include "windowhandler.h"

class QScreenScanner : public QObject
{
    Q_OBJECT
public:
    QScreenScanner(WindowHandler *w, QObject *parent = nullptr);
    QScreenScanner(WindowHandler *w, int x, int y, QObject *parent = nullptr);
    QScreenScanner(WindowHandler *w, int x, int y, int w, int h, QObject *parent = nullptr);

    enum ScanMode{
        PIXEL = 0,
        AREA,
        WINDOW
    };

signals:
    void scanned(QPixmap);
    void finished();

public slots:
    void process();
    void stop();

private:
    ScanMode mode = 0;

    bool run = true;
    int x;
    int y;
    WindowHandler *w = 0;
};

#endif // WINDOWHANDLERWORKER_H