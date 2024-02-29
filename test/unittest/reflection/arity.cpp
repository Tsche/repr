#include <librepr/reflection/detail/arity.h>

using librepr::detail::arity;

struct Foo {
  unsigned bar2[5];
  int i;
  struct {
    int a;
    char b;
  } foo;
  unsigned bar[5];
  int b;
};

static_assert(arity<Foo> == 5);

struct A {
  unsigned bar[2];
  int i;
};

struct A2 {
  int i;
  unsigned bar[2];
};

static_assert(arity<A> == 2);
static_assert(arity<A2> == 2);

struct B {
  struct {
    int a;
    int b;
  } f;
  int a;
};

struct B2 {
  int a;
  struct {
    int a;
    int b;
  } f;
};

static_assert(arity<B> == 2);
static_assert(arity<B2> == 2);

struct C {
  int a[2];
  struct {
    int a;
    int b;
  } f;
};

struct C2 {
  int a[2];
  struct {
    int a;
    int b;
  } f;
  struct {
    int a;
    int b;
  } f2;
};

static_assert(arity<C> == 2);
static_assert(arity<C2> == 3);

struct D {
  struct NonAgg {
    NonAgg(int, float, double) {}
  } f;
};

static_assert(arity<D> == 1);

#include <initializer_list>
struct E {
  struct NonAgg {
    NonAgg(std::initializer_list<int>) {}
  } f;
};

static_assert(arity<E> == 1);

/* NONSTANDARD, okay to fail
    struct F {
        int a;
        int b[];
    };
    static_assert(arity<F> == 2);
*/

struct G {
  int a;
  void* b;
};

struct G2 {
  void* b;
  int a;
};

static_assert(arity<G> == 2);
static_assert(arity<G2> == 2);

static_assert(arity<int[3]> == 3);

struct H {
  union {
    int a;
    char b[4];
  } ohno;
};

struct H2 {
  union {
    char b[4];
    int a;
  } ohno;
};

struct H3 {
  union {
    struct {
      int c;
      char d;
    } b;
    int a;
  } ohno;
};

struct H4 {
  union {
    int a;
    struct {
      int c;
      char d;
    } b;
  } ohno;
};

struct H5 {
  union {
    int a[4];
    struct {
      int c;
      char d;
    } b;
  } ohno;
};

struct H6 {
  union {
    struct {
      int c;
      char d;
    } b;
    int a[4];
  } ohno;
};

static_assert(arity<H> == 1);
static_assert(arity<H2> == 1);
static_assert(arity<H3> == 1);
static_assert(arity<H4> == 1);
static_assert(arity<H5> == 1);
static_assert(arity<H6> == 1);

struct I {
  void member_fnc();
  void member_fnc2() const;
  static void member_fnc3();
};

static_assert(arity<I> == 0);

struct J {
  void (*foo)(int);
};

struct J2 {
  struct {
    int a;
    char b;
  } bar;
  void (*foo)(int);
};

struct J3 {
  void (*foo)(int);
  struct {
    int a;
    char b;
  } bar;
};

struct J4 {
  int a[4];
  void (*foo)(int);
};

struct J5 {
  void (*foo)(int);
  int a[4];
};

static_assert(arity<J> == 1);
static_assert(arity<J2> == 2);
static_assert(arity<J3> == 2);
static_assert(arity<J4> == 2);
static_assert(arity<J5> == 2);

struct K {
  int const& b;
};

struct K2 {
  int&& b;
};

struct K3 {
  int& b;
};

static_assert(arity<K> == 1);
static_assert(arity<K2> == 1);
// static_assert(arity<K3> == 1); //TODO

#include <memory>
struct L {
  std::unique_ptr<int> a;
  std::shared_ptr<int> b;
};

static_assert(arity<L> == 2);
