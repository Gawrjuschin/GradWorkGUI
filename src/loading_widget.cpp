#include "loading_widget.h"

#include <QApplication>
#include <QGraphicsProxyWidget>
#include <QGraphicsView>
#include <QLabel>
#include <QPixmap>
#include <QTimer>
#include <QVBoxLayout>

constexpr int interval_p_text = 1000;
constexpr int interval_pic = 1000 / 33;
constexpr int angle = 10;

Loading_Widget::Loading_Widget(const int pix_size, QWidget* parent)
    : QWidget(parent)
    , p_text(new QLabel(tr("Loading")))
    , p_pic_lbl(new QLabel)
    , m_pix_size(pix_size)
    , p_timer_text(new QTimer(this))
    , p_timer_pic(new QTimer(this))
{
  auto* main_lo = new QVBoxLayout(this);
  main_lo->setContentsMargins({0, 0, 0, 0});
  main_lo->setAlignment(Qt::AlignCenter);

  auto* subwidget = new QWidget(this);
  main_lo->addStretch(1);
  main_lo->addWidget(subwidget, 0);
  main_lo->addStretch(1);
  p_text->setAlignment(Qt::AlignCenter);
  p_text->setFixedWidth(m_pix_size);

  auto* sub_lo = new QVBoxLayout(subwidget);
  sub_lo->setAlignment(Qt::AlignCenter);
  sub_lo->setContentsMargins({0, 0, 0, 0});
  sub_lo->addWidget(p_text);

  p_pic_lbl->setObjectName(QString::fromLatin1("PicLabel"));
  p_pic_lbl->setAttribute(Qt::WA_TranslucentBackground);
  auto* scene = new QGraphicsScene(0, 0, m_pix_size, m_pix_size);
  p_proxy = scene->addWidget(p_pic_lbl);

  auto* view(new QGraphicsView(scene));
  view->setFixedSize(m_pix_size, m_pix_size);
  view->setRenderHint(QPainter::Antialiasing);
  sub_lo->addWidget(view);

  connect(p_timer_text, &QTimer::timeout, this, &Loading_Widget::onNextText);
  connect(p_timer_pic, &QTimer::timeout, this, &Loading_Widget::onNextPic);
}

Loading_Widget::~Loading_Widget() = default;

void Loading_Widget::rotate_pic(double angle)
{
  static auto dx = p_proxy->scene()->sceneRect().width() / 2;
  static auto dy = p_proxy->scene()->sceneRect().height() / 2;
  QTransform transform = p_proxy->transform();
  transform.translate(dx, dx);
  transform.rotate(angle, Qt::ZAxis);
  transform.translate(-dx, -dy);
  p_proxy->setTransform(transform);
}

void Loading_Widget::onSetPixmap(const QPixmap& pic) {
  p_pic_lbl->setPixmap(pic.scaledToHeight(m_pix_size, Qt::TransformationMode::SmoothTransformation));
  emit pixmapChanged();
}

QPixmap Loading_Widget::pixmap() const
{
  return p_pic_lbl->pixmap(Qt::ReturnByValueConstant{});
}

void Loading_Widget::onStop() {
  p_timer_text->stop();
  p_timer_pic->stop();
  m_stage = 0;
  QTransform transform = p_proxy->transform();
  transform.reset();
  p_proxy->setTransform(transform);
}

void Loading_Widget::onStart() {
  p_timer_text->start(0);
  p_timer_pic->start(0);
}

void Loading_Widget::onNextText() {
  switch (m_stage) {
    case 0:
      p_text->setText(tr("Loading"));
      break;
    case 1:
      p_text->setText(tr("Loading."));
      break;
    case 2:
      p_text->setText(tr("Loading.."));
      break;
    case 3:
      p_text->setText(tr("Loading..."));
      break;
  }
  ++m_stage;
  m_stage %= 4;
  p_timer_text->start(interval_p_text);
}

void Loading_Widget::onNextPic() {
  rotate_pic(angle);
  p_timer_pic->start(interval_pic);
}
