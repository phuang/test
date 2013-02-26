#include <QObject>

class A : public QObject {
  Q_OBJECT
  Q_PROPERTY(int value READ setValue WRITE getValue)

 public slots:
  void setValue(int i);
  int getValue();
};

class Counter : public QObject
{
  Q_OBJECT
 public:
  enum CounterFlags {
    One = 1,
    Two = 2,
  };
  Q_ENUMS(CounterFlags);
  Q_FLAGS(CounterFlags);

 
 public slots:
  struct A valueA() const { return m_a; }
  const struct A& valueAR() const { return m_a; }
  const struct A* valueAP() const { return &m_a; }
  long long valueLL() const { return (long long) m_value; }
  int value() const { return m_value; }
  virtual void setValue(int value);

  void setFlags(CounterFlags flags) { m_flags = flags; }
 
 signals:
  void valueChanged(int newValue);
 
 private:
  Q_PROPERTY(int value READ value WRITE setValue DESIGNABLE true);
  int m_value;
  struct A m_a;
  CounterFlags m_flags;
};

class CounterEx : public Counter
{
  Q_OBJECT
 public slots:
  virtual void setValue(int newValue);
};

class AA: public virtual QObject, public virtual Counter {
  Q_OBJECT
};

