#ifndef _COMMUNICATION_H
#define _COMMUNICATION_H


class Communication : public typedef2 {
  Q_OBJECT
  public:
    explicit Communication(QWidget * parent = 0);

    ~Communication();

  public slots:
  private:
    QBluetoothSocket * socket;

};
#endif
