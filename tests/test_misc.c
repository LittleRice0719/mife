#include <gghlite/gghlite.h>
#include <gghlite/gghlite-internals.h>
#include <flint-addons/cyclotomic-2power.h>

int test_inv_approx(long n, long prec) {
  flint_rand_t randstate;
  flint_randinit_seed(randstate, 0x1337, 1);
  
  fmpz_poly_t f;
  fmpz_poly_init(f);

  fmpq_poly_t modulus_q; fmpq_poly_init(modulus_q);
  fmpq_poly_set_coeff_si(modulus_q, 0, 1);
  fmpq_poly_set_coeff_si(modulus_q, n, 1);
  fmpz_poly_t modulus_z; fmpz_poly_init(modulus_z);
  fmpz_poly_set_coeff_si(modulus_z, 0, 1);
  fmpz_poly_set_coeff_si(modulus_z, n, 1);

  mpfr_t sigma;
  mpfr_init2(sigma, prec);
  mpfr_set_d(sigma, _gghlite_sigma(n), MPFR_RNDN);
  
  fmpz_poly_sample_sigma(f, n, sigma, randstate);

  fmpq_poly_t f_q; fmpq_poly_init(f_q);
  fmpq_poly_set_fmpz_poly(f_q, f);
  fmpq_poly_t f_inv1; fmpq_poly_init(f_inv1);
  uint64_t t1 = ggh_walltime(0);
  _fmpq_poly_invert_mod_cnf2pow_approx(f_inv1, f_q, n, prec);
  t1 = ggh_walltime(t1);  
  printf("CATALIN t: %7.1f\n", t1/1000000.0);

  fmpq_poly_t f_inv2; fmpq_poly_init(f_inv2);
  uint64_t t2 = ggh_walltime(0);
  fmpq_poly_invert_mod_cnf2pow_approx(f_inv2, f_q, n, prec);
  t2 = ggh_walltime(t2);  
  printf("ITERCAT t: %7.1f\n", t2/1000000.0);

  fmpq_poly_t f_inv0; fmpq_poly_init(f_inv0);  
  uint64_t t0 = ggh_walltime(0);
  fmpz_poly_invert_mod_fmpq(f_inv0, f, modulus_z);
  t0 = ggh_walltime(t0);
  printf("  FLINT t: %7.1f\n", t0/1000000.0);
  return 0;
}



int main(int argc, char *argv[]) {
  assert(argc>=3);
  const long n    = atol(argv[1]);
  const long prec = atol(argv[2]);


  flint_rand_t randstate;
  flint_randinit_seed(randstate, 0x1337, 1);
  
  fmpz_poly_t f;
  fmpz_poly_init(f);

  mpfr_t sigma;
  mpfr_init2(sigma, prec);
  mpfr_set_d(sigma, _gghlite_sigma(n), MPFR_RNDN);
  
  fmpz_poly_sample_sigma(f, n, sigma, randstate);
  fmpq_poly_t g;
  fmpq_poly_init(g);
  fmpq_poly_set_fmpz_poly(g, f);

  fmpq_poly_t g_sqrt;
  fmpq_poly_init(g_sqrt);
  
  fmpq_poly_sqrt_mod_cnf2power_approx(g_sqrt, g, n, prec);

  
  return 0;
}
