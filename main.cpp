//
//  main.cpp
//  CF.RandomNG01
//
//  Created by Alan Sampson on 11/3/23.
//

#include <iostream>
#include <iomanip>
#include <string>
#include <map>
#include <algorithm>
#include <utility>
#include <random>
#include <cstdlib>
#include <unistd.h>
#include <sys/random.h>

#include "identify.h"
#include "version_info.h"

//  MARK: - Definitions
using namespace std::literals::string_literals;

//  MARK: Enumreration  rfun
enum rfun {
  f_rand, f_random, f_rand_r,
#ifndef __APPLE__
  f_getrandom,
#endif /* __APPLE__ */
  f_getentropy,
  a_lce0, a_lce,
  a_mt32, a_mt64,
  a_ranlux24_base, a_ranlux48_base,
  a_ranlux24, a_ranlux48,
  a_knuth_b,
  a_ibe_mt,
  a_default,
  d_uniform_int,
};

void rando(rfun rfn);

//  MARK: - Implementation
/*
 *  MARK: main()
 */
int main(int argc, const char * argv[]) {
  std::cout << "CF.RandomNG01\n"s;

  std::cout << std::string(60, '-') << '\n';
  avi::version_details();
  avi::identify();

  // --------------------------------------------------------------
  std::cout << std::string(60, '-') << '\n';
  std::cout << "rand()\n"s;
  rando(rfun::f_rand);

  std::cout << std::string(60, '-') << '\n';
  std::cout << "random()\n"s;
  rando(rfun::f_random);

#ifndef __APPLE__
  std::cout << std::string(60, '-') << '\n';
  std::cout << "getrandom()\n"s;
  rando(rfun::f_getrandom);
#endif /* __APPLE__ */

  std::cout << std::string(60, '-') << '\n';
  std::cout << "getentropy()\n"s;
  rando(rfun::f_getentropy);

  std::cout << std::string(60, '-') << '\n';
  std::cout << "rand_r()\n"s;
  rando(rfun::f_rand_r);

  std::cout << std::string(60, '-') << '\n';
  std::cout << "std::minstd_rand0\n"s;
  rando(rfun::a_lce0);

  std::cout << std::string(60, '-') << '\n';
  std::cout << "std::minstd_rand\n"s;
  rando(rfun::a_lce);

  std::cout << std::string(60, '-') << '\n';
  std::cout << "std::mt19937\n"s;
  rando(rfun::a_mt32);

  std::cout << std::string(60, '-') << '\n';
  std::cout << "std::mt19937_64\n"s;
  rando(rfun::a_mt64);

  std::cout << std::string(60, '-') << '\n';
  std::cout << "std::ranlux24_base\n"s;
  rando(rfun::a_ranlux24_base);

  std::cout << std::string(60, '-') << '\n';
  std::cout << "std::ranlux48_base\n"s;
  rando(rfun::a_ranlux48_base);

  std::cout << std::string(60, '-') << '\n';
  std::cout << "std::ranlux24\n"s;
  rando(rfun::a_ranlux24);

  std::cout << std::string(60, '-') << '\n';
  std::cout << "std::ranlux48\n"s;
  rando(rfun::a_ranlux48);

  std::cout << std::string(60, '-') << '\n';
  std::cout << "std::knuth_b\n"s;
  rando(a_knuth_b);

  std::cout << std::string(60, '-') << '\n';
  std::cout << "std::independent_bits_engine\n"s;
  rando(a_ibe_mt);

  std::cout << std::string(60, '-') << '\n';
  std::cout << "std::default_random_engine\n"s;
  rando(a_default);

  std::cout << std::string(60, '-') << '\n';
  std::cout << "std::uniform_int_distribution\n"s;
  rando(d_uniform_int);

  std::cout << "Complete.\n"s;

  return 0;
}

/*
 *  MARK: rando()
 */
void rando(rfun rfn) {
  std::cout << "In function "s << __func__ << "()\n"s;

  std::map<uint64_t, std::size_t> memo;

  auto rmin { 1 };
  auto rmax { 51 };

  auto rseed { 0u };

  size_t constexpr grbuf_e { 8ul };
  uint32_t grbuf[grbuf_e] {  1u, };
  size_t grbuf_ct { 0ul };

  //  std::linear_congruential_engine
  std::minstd_rand0 lce0;
  std::minstd_rand lce;

  //  std::mersenne_twister_engine
  std::mt19937 mt32;
  std::mt19937_64 mt64;

  //  std::subtract_with_carry_engine
  std::ranlux24_base ranlux24_sce;
  std::ranlux48_base ranlux48_sce;

  //  std::discard_block_engine
  std::ranlux24 ranlux24_dbe;
  std::ranlux48 ranlux48_dbe;

  //  std::shuffle_order_engine
  std::knuth_b knuth;

  //  std::independent_bits_engine
  std::independent_bits_engine<std::mt19937, 32, std::uint32_t> ibe_mt;

  //  default_random_engine
  std::default_random_engine dflt;

  //  std::uniform_int_distribution
  std::random_device rd;
  std::mt19937 mte(rd());
  std::uniform_int_distribution<> uid(rmin, rmax - 1);

  for (auto r_ = 0ul; r_ < 5'000; ++r_) {
    auto rv { 0ul };
    switch (rfn) {
      case rfun::f_random:
        {
          rv = ::random() % (rmax - rmin) + rmin;
        }
        break;

      case rfun::f_rand_r:
        {
          rv = ::rand_r(&rseed) % (rmax - rmin) + rmin;
        }
        break;

#ifndef __APPLE__
      case rfun::f_getrandom:
        {
          auto rrc = ::getrandom(&grbuf, sizeof(grbuf_e), 0x00);
          if (rrc > 0) {
            grbuf_ct %= grbuf_e;
            auto gr_val = grbuf[grbuf_ct++];
            rv = gr_val % (rmax - rmin) + rmin;
          }
          else if (rrc < 0) {
            rv = rmin;
          }
          else {
            rv = rmax;
          }
        }
        break;;
#endif /* __APPLE__ */

      case rfun::f_getentropy:
        {
          auto rrc = ::getentropy(&grbuf, sizeof(grbuf_e));
          if (rrc == 0) {
            grbuf_ct %= grbuf_e;
            auto gr_val = grbuf[grbuf_ct++];
            rv = gr_val % (rmax - rmin) + rmin;
          }
          else {
            rv = rmin;
          }
        }
        break;;

        //  std::linear_congruential_engine
      case rfun::a_lce0:
        {
          auto rn = lce0();
          rv = rn % (rmax - rmin) + rmin;
        }
        break;

      case rfun::a_lce:
        {
          auto rn = lce();
          rv = rn % (rmax - rmin) + rmin;
        }
        break;

      //  std::mersenne_twister_engine
      case rfun::a_mt32:
        {
          auto rn = mt32();
          rv = rn % (rmax - rmin) + rmin;
        }
        break;

      case rfun::a_mt64:
        {
          auto rn = mt64();
          rv = rn % (rmax - rmin) + rmin;
        }
        break;

      //  std::subtract_with_carry_engine
      case rfun::a_ranlux24_base:
        {
          auto rn = ranlux24_sce();
          rv = rn % (rmax - rmin) + rmin;
        }
        break;

      case rfun::a_ranlux48_base:
        {
          auto rn = ranlux48_sce();
          rv = rn % (rmax - rmin) + rmin;
        }
        break;

      //  std::discard_block_engine
      case rfun::a_ranlux24:
        {
          auto rn = ranlux24_dbe();
          rv = rn % (rmax - rmin) + rmin;
        }
        break;

      case rfun::a_ranlux48:
        {
          auto rn = ranlux48_dbe();
          rv = rn % (rmax - rmin) + rmin;
        }
        break;

      //  std::subtract_with_carry_engine
      case rfun::a_knuth_b:
        {
          auto rn = knuth();
          rv = rn % (rmax - rmin) + rmin;
        }
        break;

      //  std::independent_bits_engine
      case rfun::a_ibe_mt:
        {
          auto rn = ibe_mt();
          rv = rn % (rmax - rmin) + rmin;
        }
        break;

      //  std::default_random_engine
      case rfun::a_default:
        {
          auto rn = dflt();
          rv = rn % (rmax - rmin) + rmin;
        }
        break;

      //  std::uniform_int_distribution
      case rfun::d_uniform_int:
        {
          rv = uid(mte);
        }
        break;

      case rfun::f_rand:
      default:
        {
          rv = std::rand() % (rmax - rmin) + rmin;
        }
        break;
    }
    memo[rv] += 1ul;
  }

  auto [mn, mx] = std::minmax_element(memo.cbegin(), memo.cend(),
                  [](auto const & lhs, auto const & rhs) {
                    return lhs.second < rhs.second;
                  });
  auto minkey = (*mn).first;
  auto minval = (*mn).second;
  auto maxkey = (*mx).first;
  auto maxval = (*mx).second;
  std::cout << "min key:"s << std::setw(5) << minkey
            << ", min val:"s << std::setw(5) << minval
            << '\n';
  std::cout << "max key:"s << std::setw(5) << maxkey
            << ", max val:"s << std::setw(5) << maxval
            << '\n';

  for (auto const & [key, val] : memo) {
    std::cout << std::setw(9) << key << " : "s << std::setw(5)
              << val << " : *..."s << std::string(val - minval + 1ul, '*')
              << '\n';
  }

  std::cout << std::endl;

  return;
}