#ifndef PARTICLECONTROL_H
#define PARTICLECONTROL_H

#include <QtGui>

struct Particle;
struct ParticleEmitterPrivate;

////////////////////////////////////////////////////////////////////////////////

class ParticleCreator
{
  friend class ParticleEmitter;

public:
  ParticleCreator() {}
  virtual ~ParticleCreator() {}

protected:
  virtual void createParticle(Particle *part, ParticleEmitterPrivate *priv) = 0;
};

////////////////////////////////////////////////////////////////////////////////

class UniformParticleCreator : public ParticleCreator
{
public:
  UniformParticleCreator(int id);

protected:
  virtual void createParticle(Particle *part, ParticleEmitterPrivate *priv);

  int myId;
};

////////////////////////////////////////////////////////////////////////////////

class ParticleEmitter
{
public:
  ParticleEmitter(int x = 0, int y = 0);
  ParticleEmitter(const QRect &rect);
  virtual ~ParticleEmitter();

  void move(int x, int y);

  void setCount(int count);

  void setEmitRadius(int xradius, int yradius);

  void setOpacityShift(double shift);

  void setDuration(int time = 0);
  bool isAlive() const;
  void kill();

  bool isActive() const;
  void setActive(bool act);

  void setCreator(ParticleCreator *creator);

  virtual void draw(QPainter &p);

  virtual void advance();

protected:
  virtual void createParticle(struct Particle *part);

  ParticleCreator *myCreator;

  ParticleEmitterPrivate *priv;
};

////////////////////////////////////////////////////////////////////////////////

class ParticleControl
{
public:
    ParticleControl();
    virtual ~ParticleControl();

    void addEmitter(ParticleEmitter *emt);

    void paint(QPainter &p);

    void advance();

protected:
    struct ParticleControlPrivate *priv;
};

////////////////////////////////////////////////////////////////////////////////

#endif // PARTICLECONTROL_H
