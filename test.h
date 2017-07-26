#ifndef TEST_H
#define TEST_H


#include <QObject>

class Test : public QObject
{
    Q_OBJECT
public:
    Test();
signals:
    void msg(char * msg);

};

#endif // TEST_H
