#include "src/context.h"
#include "test/context_tests.h"
#include "test/disagreement_set_tests.h"
#include "test/term_tests.h"

using namespace stt_res;

void scratch() {
  context c;
  auto t = c.mk_tvar("t");
  auto ft = c.mk_tfunc(t, t);
  auto x = c.mk_var("x", t);
  auto f = c.mk_var("f", ft);
  auto F = c.mk_con("F", ft);
  auto fFx = c.mk_ap(f, c.mk_ap(F, x));
  auto Ffx = c.mk_ap(F, c.mk_ap(f, x));
  cout << *fFx << endl;
  cout << *Ffx << endl;
  sub s{tp(fFx, Ffx)};
  disagreement_set d(s);
  cout << d << endl;;
  c.add_imitation_binding(d);
  cout << d << endl;
  c.solve_vars(d);
  cout << d << endl;
  c.reduce_pair_args(d);
  cout << d << endl;
  auto r = c.add_projection_binding(d);
  if (!r) {
    cout << "Did not add projection binding" << endl;
  } else {
    cout << "Added projection binding" << endl;
  }
  cout << d << endl;
  c.solve_vars(d);
  cout << d << endl;
  d.delete_identical_pairs();
  cout << "FINAL d.size() == " << d.size() << endl;
  cout << d;
  auto final_sub = d.extract_sub();
  cout << "final_sub" << endl;
  cout << final_sub << endl;
  auto sFfx = c.apply_sub(final_sub, Ffx);
  auto sfFx = c.apply_sub(final_sub, fFx);
  cout << *sFfx << endl;
  cout << *sfFx << endl;
}

int main() {
  scratch();
  all_term_tests();
  all_disagreement_set_tests();
  all_context_tests();
  return 0;
}
