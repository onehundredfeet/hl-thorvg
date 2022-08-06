#include <thorvg.h>


template<class T>
struct UniqueHolder {
  UniqueHolder() {
  }
  T &get() {
    return * _ptr.get();
  }
  T *getPtr() {
    return _ptr.get();
  }
  std::unique_ptr<T> _ptr;

  static UniqueHolder<T> *make(std::unique_ptr<T> ptr) {
    auto *x = new UniqueHolder<T>();
    x->_ptr = ptr;
    return x;
  }
};

static  tvg::SwCanvas *createCanvas() {
  return tvg::SwCanvas::gen().release();
}

static  tvg::Shape *createShape() {
  return tvg::Shape::gen().release();
}

inline void thorvg_engine_init( int threadcount ) {
  printf("Initializing\n");
  tvg::Initializer::init(tvg::CanvasEngine::Sw, threadcount);
  printf("Go!\n");
}


inline void thorvg_engine_destroy( ) {
  tvg::Initializer::term(tvg::CanvasEngine::Sw);
}

inline void canvas_push_shape(tvg::SwCanvas *c, tvg::Shape *s) {
  std::unique_ptr<tvg::Paint> ptr (s);
  c->push( move(ptr) );
}


#define CS_ABGR8888 tvg::SwCanvas::ABGR8888
#define CS_ARGB8888 tvg::SwCanvas::ARGB8888
#define CS_ABGR8888_STRAIGHT tvg::SwCanvas::ABGR8888_STRAIGHT
#define CS_ARGB8888_STRAIGHT tvg::SwCanvas::ARGB8888_STRAIGHT


/*
inline void blend2_context_setFillStyleColourPacked( BLContext *ctx, unsigned int rgba ) {
    ctx->setFillStyle(BLRgba32(rgba));
}

inline void blend2_context_setFillStyleColour( BLContext *ctx, int r, int g, int b, int a ) {
    ctx->setFillStyle(BLRgba32(r,g,b,a));
}


void blah() {
    BLImage img(480, 480, BL_FORMAT_PRGB32);

  // Attach a rendering context into `img`.
  BLContext ctx(img);

  // Clear the image.
  ctx.setCompOp(BL_COMP_OP_SRC_COPY);
  ctx.fillAll();

  // Fill some path.
  BLPath path;
  path.moveTo(26, 31);
  path.cubicTo(642, 132, 587, -136, 25, 464);
  path.cubicTo(882, 404, 144, 267, 27, 31);

  ctx.setCompOp(BL_COMP_OP_SRC_OVER);
  ctx.setFillStyle(BLRgba32(0xFFFFFFFF));
//  ctx.fillPath(path);
//   ctx.fillPolygon()
  // Detach the rendering context from `img`.
  //ctx.end();

  // Let's use some built-in codecs provided by Blend2D.
  BLImageCodec codec;
  codec.findByName("PNG");
  img.writeToFile("bl-getting-started-1.png", codec);

}*/