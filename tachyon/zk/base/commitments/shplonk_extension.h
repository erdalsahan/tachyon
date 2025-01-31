// Copyright 2020-2022 The Electric Coin Company
// Copyright 2022 The Halo2 developers
// Use of this source code is governed by a MIT/Apache-2.0 style license that
// can be found in the LICENSE-MIT.halo2 and the LICENCE-APACHE.halo2
// file.

#ifndef TACHYON_ZK_BASE_COMMITMENTS_SHPLONK_EXTENSION_H_
#define TACHYON_ZK_BASE_COMMITMENTS_SHPLONK_EXTENSION_H_

#include <stddef.h>

#include <utility>
#include <vector>

#include "tachyon/crypto/commitments/batch_commitment_state.h"
#include "tachyon/crypto/commitments/kzg/shplonk.h"
#include "tachyon/zk/base/commitments/univariate_polynomial_commitment_scheme_extension.h"

namespace tachyon {
namespace zk {

template <typename Curve, size_t MaxDegree, size_t MaxExtendedDegree,
          typename Commitment>
class SHPlonkExtension final
    : public UnivariatePolynomialCommitmentSchemeExtension<
          SHPlonkExtension<Curve, MaxDegree, MaxExtendedDegree, Commitment>> {
 public:
  // NOTE(dongchangYoo): The following value are pre-determined according to
  // the Commitment Opening Scheme.
  // https://
  // github.com/kroma-network/halo2/blob/7d0a36990452c8e7ebd600de258420781a9b7917/halo2_proofs/src/poly/kzg/multiopen/shplonk/prover.rs#L111
  constexpr static bool kQueryInstance = false;

  using Base = UnivariatePolynomialCommitmentSchemeExtension<
      SHPlonkExtension<Curve, MaxDegree, MaxExtendedDegree, Commitment>>;
  using Field = typename Base::Field;
  using Poly = typename Base::Poly;
  using Evals = typename Base::Evals;

  SHPlonkExtension() = default;
  explicit SHPlonkExtension(
      crypto::SHPlonk<Curve, MaxDegree, Commitment>&& shplonk)
      : shplonk_(std::move(shplonk)) {}

  size_t N() const { return shplonk_.N(); }

  size_t D() const { return N() - 1; }

  crypto::BatchCommitmentState& batch_commitment_state() {
    return shplonk_.batch_commitment_state();
  }
  bool GetBatchMode() const { return shplonk_.GetBatchMode(); }

  void SetBatchMode(size_t batch_count) { shplonk_.SetBatchMode(batch_count); }

  std::vector<Commitment> GetBatchCommitments() {
    return shplonk_.GetBatchCommitments();
  }

  [[nodiscard]] bool DoUnsafeSetup(size_t size) {
    return shplonk_.DoUnsafeSetup(size);
  }

  [[nodiscard]] bool DoUnsafeSetup(size_t size, const Field& tau) {
    return shplonk_.DoUnsafeSetup(size, tau);
  }

  template <typename ScalarContainer>
  [[nodiscard]] bool DoCommit(const ScalarContainer& v, Commitment* out) const {
    return shplonk_.DoCommit(v, out);
  }

  template <typename ScalarContainer>
  [[nodiscard]] bool DoCommit(const ScalarContainer& v,
                              crypto::BatchCommitmentState& state,
                              size_t index) {
    return shplonk_.DoCommit(v, state, index);
  }

  [[nodiscard]] bool DoCommit(const Poly& poly, Commitment* out) const {
    return shplonk_.DoCommit(poly, out);
  }

  [[nodiscard]] bool DoCommit(const Poly& poly,
                              crypto::BatchCommitmentState& state,
                              size_t index) {
    return shplonk_.DoCommit(poly, state, index);
  }

  [[nodiscard]] bool DoCommitLagrange(const Evals& evals,
                                      Commitment* out) const {
    return shplonk_.DoCommitLagrange(evals, out);
  }

  [[nodiscard]] bool DoCommitLagrange(const Evals& evals,
                                      crypto::BatchCommitmentState& state,
                                      size_t index) {
    return shplonk_.DoCommitLagrange(evals, state, index);
  }

  template <typename ScalarContainer>
  [[nodiscard]] bool DoCommitLagrange(const ScalarContainer& v,
                                      Commitment* out) const {
    return shplonk_.DoCommitLagrange(v, out);
  }

  template <typename ScalarContainer>
  [[nodiscard]] bool DoCommitLagrange(const ScalarContainer& v,
                                      crypto::BatchCommitmentState& state,
                                      size_t index) {
    return shplonk_.DoCommitLagrange(v, state, index);
  }

  template <typename Container, typename Proof>
  [[nodiscard]] bool DoCreateOpeningProof(const Container& poly_openings,
                                          Proof* proof) const {
    return shplonk_.DoCreateOpeningProof(poly_openings, proof);
  }

  template <typename Container, typename Proof>
  [[nodiscard]] bool DoVerifyOpeningProof(const Container& poly_openings,
                                          Proof* proof) const {
    return shplonk_.DoVerifyOpeningProof(poly_openings, proof);
  }

 private:
  crypto::SHPlonk<Curve, MaxDegree, Commitment> shplonk_;
};

template <typename Curve, size_t MaxDegree, size_t MaxExtendedDegree,
          typename Commitment>
struct UnivariatePolynomialCommitmentSchemeExtensionTraits<
    SHPlonkExtension<Curve, MaxDegree, MaxExtendedDegree, Commitment>> {
 public:
  constexpr static size_t kMaxExtendedDegree = MaxExtendedDegree;
  constexpr static size_t kMaxExtendedSize = kMaxExtendedDegree + 1;
};

}  // namespace zk

namespace crypto {

template <typename Curve, size_t MaxDegree, size_t MaxExtendedDegree,
          typename _Commitment>
struct VectorCommitmentSchemeTraits<
    zk::SHPlonkExtension<Curve, MaxDegree, MaxExtendedDegree, _Commitment>> {
 public:
  using G1Point = typename Curve::G1Curve::AffinePoint;
  using Field = typename G1Point::ScalarField;
  using Commitment = _Commitment;

  constexpr static size_t kMaxSize = MaxDegree + 1;
  constexpr static bool kIsTransparent = false;
  constexpr static bool kSupportsBatchMode = true;
};

}  // namespace crypto
}  // namespace tachyon

#endif  // TACHYON_ZK_BASE_COMMITMENTS_SHPLONK_EXTENSION_H_
