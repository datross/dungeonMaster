#ifndef TRAP_H
#define TRAP_H

class Trap {
private:
  int damage;
  uint activationTiming;
public:
  void activate();
  void reset();
  void detect();
};

#endif //TRAP_H
