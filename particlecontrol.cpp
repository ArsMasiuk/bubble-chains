#include "particlecontrol.h"

////////////////////////////////////////////////////////////////////////////////

struct Particle
{
  Particle()
  {
//    x = y = 0;
//    dx = dy = 0;
//    count = 100;
//    opacity = 1;
//    dop = opacity/count;
//    color = Qt::white;
  }

  double x,y;
  double dx,dy;
  int count;
  double opacity;
  double dop;
  QColor color;
  QImage *shape;
};

////////////////////////////////////////////////////////////////////////////////

#define SHAPE_COUNT 7
#define SHAPE_SIZE  48

struct ParticleEmitterPrivate
{
  ParticleEmitterPrivate() :
      x(0), y(0),
      xradius(0), yradius(0),
      count(10),
      act(true),
      time(0),
      op_shift(1),
      creator(0)
  {
    if (shape[0] == 0)
    {
      const int colors[] = {
        0x0000ff, // blue
        0xffff00, // yellow
        0xff0000, // red
        0x00ff00, // green
        0xff00ff, // magenta
        0x00ffff, // cyan
        0xffffff  // white
      };

      QImage alpha = QImage(":/images/flare.png").scaled(SHAPE_SIZE,SHAPE_SIZE);

      for (int i = 0; i < SHAPE_COUNT; i++)
      {
        shape[i] = new QImage(SHAPE_SIZE,SHAPE_SIZE,QImage::Format_RGB32);
        shape[i]->fill(colors[i]);
        shape[i]->setAlphaChannel(alpha);
      }
    }
  }

  ~ParticleEmitterPrivate()
  {
    if (creator)
      delete creator;
  }

  int x,y;
  int xradius, yradius;
  int count;

  bool act;
  int time;

  double op_shift;

  QList<Particle> particles;

  ParticleCreator *creator;

  static QImage* shape[SHAPE_COUNT];
};

QImage* ParticleEmitterPrivate::shape[SHAPE_COUNT] = {0};

ParticleEmitter::ParticleEmitter(int x, int y)
{
  priv = new ParticleEmitterPrivate();
  priv->x = x;
  priv->y = y;
}

ParticleEmitter::ParticleEmitter(const QRect &rect)
{
  priv = new ParticleEmitterPrivate();
  priv->x = rect.center().x();
  priv->y = rect.center().y();
  priv->xradius = rect.width()/2;
  priv->yradius = rect.height()/2;
}

ParticleEmitter::~ParticleEmitter()
{
  // kill particles
  delete priv;
}

void ParticleEmitter::move(int x, int y)
{
  priv->x = x;
  priv->y = y;
}

void ParticleEmitter::setCount(int count)
{
  priv->count = count;
}

void ParticleEmitter::setEmitRadius(int xradius, int yradius)
{
  priv->xradius = xradius;
  priv->yradius = yradius;
}

void ParticleEmitter::setOpacityShift(double shift)
{
  priv->op_shift = shift;
}

bool ParticleEmitter::isActive() const
{
  return priv->act;
}

void ParticleEmitter::setActive(bool act)
{
  priv->act = act;
}

void ParticleEmitter::setDuration(int time)
{
  priv->time = time;
}

bool ParticleEmitter::isAlive() const
{
  return priv->time >= 0;
}

void ParticleEmitter::kill()
{
  priv->time = -1;
}

void ParticleEmitter::setCreator(ParticleCreator *creator)
{
  if (priv->creator)
    delete priv->creator;

  priv->creator = creator;
}

void ParticleEmitter::draw(QPainter &p)
{
  for (int i = 0; i < priv->particles.count(); i++)
  {
    const Particle &part = priv->particles.at(i);
    p.setOpacity(part.opacity);
    p.drawImage(part.x,part.y, *part.shape);
  }
}

void ParticleEmitter::advance()
{
  if (priv->time < 0)
    return;

  for (int i = 0; i < priv->particles.count(); i++)
  {
    Particle *pr = const_cast<Particle*>(& priv->particles.at(i) );
    if (--pr->count <= 0)
    {
      if (i < priv->count)
        createParticle(pr);
      else {
        priv->particles.removeAt(i);
        i--;
      }
    } else
    {
      pr->opacity += pr->dop;
      pr->x += pr->dx;
      pr->y += pr->dy;
    }
  }

  while (priv->particles.count() < priv->count)
  {
    Particle part;
    createParticle(&part);
    priv->particles.append(part);
  }

  if (priv->time > 0)
    if (!--priv->time)
      priv->time = -1;
}

void ParticleEmitter::createParticle(Particle *part)
{
  // basic creation
  part->x = priv->x + (qrand() % (priv->xradius*2)) - priv->xradius;
  part->y = priv->y + (qrand() % (priv->yradius*2)) - priv->yradius;

  part->dx = ((qrand() % 1000)-500.)/500.;
  part->dy = ((qrand() % 1000)-500.)/500.;

  part->count = priv->time;

  if (priv->op_shift < 1)
    part->opacity = 1. - (priv->op_shift * (qrand() % 100)) / 100.;
  else
    part->opacity = 1;

  part->dop = -(part->opacity/(double)part->count);

  part->color = qRgb(qrand() % 255, qrand() % 255, qrand() % 255);

  part->shape = priv->shape[qrand() % SHAPE_COUNT];

  // modify or create via creator
  if (priv->creator)
    priv->creator->createParticle(part, priv);
}

////////////////////////////////////////////////////////////////////////////////

struct ParticleControlPrivate
{
  QList<ParticleEmitter*> emitters;
};

ParticleControl::ParticleControl()
{
  priv = new ParticleControlPrivate();
}

ParticleControl::~ParticleControl()
{
  qDeleteAll(priv->emitters);
  delete priv;
}

void ParticleControl::addEmitter(ParticleEmitter *emt)
{
  if (!emt)
    return;
  if (priv->emitters.contains(emt))
    return;

  priv->emitters.append(emt);
}

void ParticleControl::paint(QPainter &p)
{
  for (int i = 0; i < priv->emitters.count(); i++)
  {
    ParticleEmitter *emt = priv->emitters.at(i);
    if (emt->isActive())
      emt->draw(p);
  }
}

void ParticleControl::advance()
{
  for (int i = 0; i < priv->emitters.count(); i++)
  {
    ParticleEmitter *emt = priv->emitters.at(i);
    if (!emt->isAlive())
    {
      delete priv->emitters.takeAt(i);
      i--;
      continue;
    }

    if (emt->isActive())
      emt->advance();
  }
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

UniformParticleCreator::UniformParticleCreator(int id) :
    ParticleCreator(),
    myId(id)
{

}

void UniformParticleCreator::createParticle(Particle *part, ParticleEmitterPrivate *priv)
{
  // shape
  if (myId >= 0 && myId < SHAPE_COUNT)
    part->shape = priv->shape[myId];
}

////////////////////////////////////////////////////////////////////////////////
