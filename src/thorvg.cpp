#ifdef EMSCRIPTEN

#include <emscripten.h>
#define HL_PRIM
#define HL_NAME(n)	EMSCRIPTEN_KEEPALIVE eb_##n
#define DEFINE_PRIM(ret, name, args)
#define _OPT(t) t*
#define _GET_OPT(value,t) *value
#define alloc_ref(r, _) r
#define alloc_ref_const(r,_) r
#define _ref(t)			t
#define _unref(v)		v
#define free_ref(v) delete (v)
#define HL_CONST const

#else

#define HL_NAME(x) thorvg_##x
#include <hl.h>

// Need to link in helpers
HL_API hl_type hltx_ui16;
HL_API hl_type hltx_ui8;

#define _IDL _BYTES
#define _OPT(t) vdynamic *
#define _GET_OPT(value,t) (value)->v.t
template <typename T> struct pref {
	void (*finalize)( pref<T> * );
	T *value;
};

#define _ref(t) pref<t>
#define _unref(v) v->value
#define _unref_ptr_safe(v) (v != nullptr ? v->value : nullptr)
#define alloc_ref(r,t) _alloc_ref(r,finalize_##t)
#define alloc_ref_const(r, _) _alloc_const(r)
#define HL_CONST

template<typename T> void free_ref( pref<T> *r ) {
	if( !r->finalize ) hl_error("delete() is not allowed on const value.");
	delete r->value;
	r->value = NULL;
	r->finalize = NULL;
}

template<typename T> void free_ref( pref<T> *r, void (*deleteFunc)(T*) ) {
	if( !r->finalize ) hl_error("delete() is not allowed on const value.");
	deleteFunc( r->value );
	r->value = NULL;
	r->finalize = NULL;
}

// Float vector
struct _hl_float2 {
	float x;
	float y;
};

struct _hl_float3 {
	float x;
	float y;
	float z;
};

struct _hl_float4 {
	float x;
	float y;
	float z;
	float w;
};

// int vector
struct _hl_int2 {
	int x;
	int y;
};

struct _hl_int3 {
	int x;
	int y;
	int z;
};

struct _hl_int4 {
	int x;
	int y;
	int z;
	int w;
};

// double vector
struct _hl_double2 {
	double x;
	double y;
};

struct _hl_double3 {
	double x;
	double y;
	double z;
};

struct _hl_double4 {
	double x;
	double y;
	double z;
	double w;
};

inline void testvector(_hl_float3 *v) {
  printf("v: %f %f %f\n", v->x, v->y, v->z);
}
template<typename T> pref<T> *_alloc_ref( T *value, void (*finalize)( pref<T> * ) ) {
	if (value == nullptr) return nullptr;
	pref<T> *r = (pref<T>*)hl_gc_alloc_finalizer(sizeof(pref<T>));
	r->finalize = finalize;
	r->value = value;
	return r;
}

template<typename T> pref<T> *_alloc_const( const T *value ) {
	if (value == nullptr) return nullptr;
	pref<T> *r = (pref<T>*)hl_gc_alloc_noptr(sizeof(pref<T>));
	r->finalize = NULL;
	r->value = (T*)value;
	return r;
}

inline static varray* _idc_alloc_array(float *src, int count) {
	if (src == nullptr) return nullptr;

	varray *a = NULL;
	float *p;
	a = hl_alloc_array(&hlt_f32, count);
	p = hl_aptr(a, float);

	for (int i = 0; i < count; i++) {
		p[i] = src[i];
	}
	return a;
}
inline static varray* _idc_alloc_array(unsigned char *src, int count) {
	if (src == nullptr) return nullptr;

	varray *a = NULL;
	float *p;
	a = hl_alloc_array(&hltx_ui8, count);
	p = hl_aptr(a, float);

	for (int i = 0; i < count; i++) {
		p[i] = src[i];
	}
	return a;
}

inline static varray* _idc_alloc_array( char *src, int count) {
	return _idc_alloc_array((unsigned char *)src, count);
}

inline static varray* _idc_alloc_array(int *src, int count) {
	if (src == nullptr) return nullptr;

	varray *a = NULL;
	int *p;
	a = hl_alloc_array(&hlt_i32, count);
	p = hl_aptr(a, int);

	for (int i = 0; i < count; i++) {
		p[i] = src[i];
	}
	return a;

}

inline static varray* _idc_alloc_array(double *src, int count) {
	if (src == nullptr) return nullptr;

	varray *a = NULL;
	double *p;
	a = hl_alloc_array(&hlt_f64, count);
	p = hl_aptr(a, double);

	for (int i = 0; i < count; i++) {
		p[i] = src[i];
	}
	return a;
}


inline static varray* _idc_alloc_array(const unsigned short *src, int count) {
	if (src == nullptr) return nullptr;

	varray *a = NULL;
	unsigned short *p;
	a = hl_alloc_array(&hltx_ui16, count);
	p = hl_aptr(a, unsigned short);

	for (int i = 0; i < count; i++) {
		p[i] = src[i];
	}
	return a;
}

inline static varray* _idc_alloc_array(unsigned short *src, int count) {
	if (src == nullptr) return nullptr;

	varray *a = NULL;
	unsigned short *p;
	a = hl_alloc_array(&hltx_ui16, count);
	p = hl_aptr(a, unsigned short);

	for (int i = 0; i < count; i++) {
		p[i] = src[i];
	}
	return a;
}

inline static void _idc_copy_array( float *dst, varray *src, int count) {
	float *p = hl_aptr(src, float);
	for (int i = 0; i < count; i++) {
		dst[i] = p[i];
	}
}

inline static void _idc_copy_array( varray *dst, float *src,  int count) {
	float *p = hl_aptr(dst, float);
	for (int i = 0; i < count; i++) {
		p[i] = src[i];
	}
}


inline static void _idc_copy_array( int *dst, varray *src, int count) {
	int *p = hl_aptr(src, int);
	for (int i = 0; i < count; i++) {
		dst[i] = p[i];
	}
}

inline static void _idc_copy_array( unsigned short *dst, varray *src) {
	unsigned short *p = hl_aptr(src, unsigned short);
	for (int i = 0; i < src->size; i++) {
		dst[i] = p[i];
	}
}

inline static void _idc_copy_array( const unsigned short *cdst, varray *src) {
	unsigned short *p = hl_aptr(src, unsigned short);
	unsigned short *dst = (unsigned short *)cdst;
	for (int i = 0; i < src->size; i++) {
		dst[i] = p[i];
	}
}

inline static void _idc_copy_array( varray *dst, int *src,  int count) {
	int *p = hl_aptr(dst, int);
	for (int i = 0; i < count; i++) {
		p[i] = src[i];
	}
}


inline static void _idc_copy_array( double *dst, varray *src, int count) {
	double *p = hl_aptr(src, double);
	for (int i = 0; i < count; i++) {
		dst[i] = p[i];
	}
}

inline static void _idc_copy_array( varray *dst, double *src,  int count) {
	double *p = hl_aptr(dst, double);
	for (int i = 0; i < count; i++) {
		p[i] = src[i];
	}
}

#endif

#ifdef _WIN32
#pragma warning(disable:4305)
#pragma warning(disable:4244)
#pragma warning(disable:4316)
#endif


#include <hl.h>
#include "hl-thorvg.h"





extern "C" {

static tvg::SwCanvas::Colorspace ColorSpace__values[] = { CS_ABGR8888,CS_ARGB8888,CS_ABGR8888_STRAIGHT,CS_ARGB8888_STRAIGHT };
HL_PRIM int HL_NAME(ColorSpace_toValue0)( int idx ) {
	return ColorSpace__values[idx];
}
DEFINE_PRIM(_I32, ColorSpace_toValue0, _I32);
HL_PRIM int HL_NAME(ColorSpace_indexToValue0)( int idx ) {
	return ColorSpace__values[idx];
}
DEFINE_PRIM(_I32, ColorSpace_indexToValue0, _I32);
HL_PRIM int HL_NAME(ColorSpace_valueToIndex0)( int value ) {
	for( int i = 0; i < 4; i++ ) if ( value == (int)ColorSpace__values[i]) return i; return -1;
}
DEFINE_PRIM(_I32, ColorSpace_valueToIndex0, _I32);
static void finalize_Canvas( _ref(tvg::SwCanvas)* _this ) { free_ref(_this ); }
HL_PRIM void HL_NAME(Canvas_delete)( _ref(tvg::SwCanvas)* _this ) {
	free_ref(_this );
}
DEFINE_PRIM(_VOID, Canvas_delete, _IDL);
HL_PRIM void HL_NAME(Engine_init1)(int threadCount) {
	(thorvg_engine_init(threadCount));
}
DEFINE_PRIM(_VOID, Engine_init1, _I32);

HL_PRIM void HL_NAME(Engine_destroy0)() {
	(thorvg_engine_destroy());
}
DEFINE_PRIM(_VOID, Engine_destroy0,);

HL_PRIM HL_CONST _ref(tvg::Shape)* HL_NAME(Shape_make0)() {
	return alloc_ref_const((createShape()),Shape);
}
DEFINE_PRIM(_IDL, Shape_make0,);

HL_PRIM void HL_NAME(Shape_moveTo2)(_ref(tvg::Shape)* _this, float x, float y) {
	(_unref(_this)->moveTo(x, y));
}
DEFINE_PRIM(_VOID, Shape_moveTo2, _IDL _F32 _F32);

HL_PRIM void HL_NAME(Shape_lineTo2)(_ref(tvg::Shape)* _this, float x, float y) {
	(_unref(_this)->lineTo(x, y));
}
DEFINE_PRIM(_VOID, Shape_lineTo2, _IDL _F32 _F32);

HL_PRIM void HL_NAME(Shape_appendRect6)(_ref(tvg::Shape)* _this, float x, float y, float width, float height, float rx, float ry) {
	(_unref(_this)->appendRect(x, y, width, height, rx, ry));
}
DEFINE_PRIM(_VOID, Shape_appendRect6, _IDL _F32 _F32 _F32 _F32 _F32 _F32);

HL_PRIM void HL_NAME(Shape_fill4)(_ref(tvg::Shape)* _this, int r, int g, int b, int a) {
	(_unref(_this)->fill(r, g, b, a));
}
DEFINE_PRIM(_VOID, Shape_fill4, _IDL _I32 _I32 _I32 _I32);

HL_PRIM void HL_NAME(Shape_close0)(_ref(tvg::Shape)* _this) {
	(_unref(_this)->close());
}
DEFINE_PRIM(_VOID, Shape_close0, _IDL);

HL_PRIM _ref(tvg::SwCanvas)* HL_NAME(Canvas_make0)() {
	return alloc_ref((createCanvas()),Canvas);
}
DEFINE_PRIM(_IDL, Canvas_make0,);

HL_PRIM void HL_NAME(Canvas_target5)(_ref(tvg::SwCanvas)* _this, vbyte* buffer, int stride, int width, int height, int colorSpace) {
	(_unref(_this)->target((uint32_t *)buffer, stride, width, height, ColorSpace__values[colorSpace]));
}
DEFINE_PRIM(_VOID, Canvas_target5, _IDL _BYTES _I32 _I32 _I32 _I32);

HL_PRIM void HL_NAME(Canvas_pushShape1)(_ref(tvg::SwCanvas)* _this, _ref(tvg::Shape)* s) {
	(canvas_push_shape( _unref(_this) , _unref_ptr_safe(s)));
}
DEFINE_PRIM(_VOID, Canvas_pushShape1, _IDL _IDL);

HL_PRIM void HL_NAME(Canvas_draw0)(_ref(tvg::SwCanvas)* _this) {
	(_unref(_this)->draw());
}
DEFINE_PRIM(_VOID, Canvas_draw0, _IDL);

HL_PRIM void HL_NAME(Canvas_sync0)(_ref(tvg::SwCanvas)* _this) {
	(_unref(_this)->sync());
}
DEFINE_PRIM(_VOID, Canvas_sync0, _IDL);

}
