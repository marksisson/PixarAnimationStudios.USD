//
// Copyright 2016 Pixar
//
// Licensed under the Apache License, Version 2.0 (the "Apache License")
// with the following modification; you may not use this file except in
// compliance with the Apache License and the following modification to it:
// Section 6. Trademarks. is deleted and replaced with:
//
// 6. Trademarks. This License does not grant permission to use the trade
//    names, trademarks, service marks, or product names of the Licensor
//    and its affiliates, except as required to comply with Section 4(c) of
//    the License and to reproduce the content of the NOTICE file.
//
// You may obtain a copy of the Apache License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the Apache License with the above modification is
// distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied. See the Apache License for the specific
// language governing permissions and limitations under the Apache License.
//
////////////////////////////////////////////////////////////////////////
// This file is generated by a script.  Do not edit directly.  Edit the
// quat.template.h file to make changes.

#ifndef PXR_BASE_GF_QUATH_H
#define PXR_BASE_GF_QUATH_H

/// \file gf/quath.h
/// \ingroup group_gf_LinearAlgebra

#include "pxr/pxr.h"
#include "pxr/base/gf/api.h"
#include "pxr/base/gf/declare.h"
#include "pxr/base/gf/vec3h.h"
#include "pxr/base/gf/traits.h"
#include "pxr/base/gf/half.h"

#include <boost/functional/hash.hpp>

#include <iosfwd>

PXR_NAMESPACE_OPEN_SCOPE

template <>
struct GfIsGfQuat<class GfQuath> { static const bool value = true; };


/// Return the dot (inner) product of two quaternions.
GfHalf GfDot(const GfQuath& q1, const GfQuath& q2);


/// \class GfQuath
/// \ingroup group_gf_LinearAlgebra
///
/// Basic type: a quaternion, a complex number with a real coefficient and
/// three imaginary coefficients, stored as a 3-vector.
///
class GfQuath
{
  public:
    typedef GfHalf ScalarType;
    typedef GfVec3h ImaginaryType;

    /// Default constructor leaves the quaternion undefined.
    GfQuath() {}

    /// Initialize the real coefficient to \p realVal and the imaginary
    /// coefficients to zero.
    ///
    /// Since quaternions typically must be normalized, reasonable values for
    /// \p realVal are -1, 0, or 1.  Other values are legal but are likely to
    /// be meaningless.
    ///
    explicit GfQuath (GfHalf realVal) : _imaginary(0), _real(realVal) {}

    /// Initialize the real and imaginary coefficients.
    GfQuath(GfHalf real, GfHalf i, GfHalf j, GfHalf k)
        : _imaginary(i, j, k), _real(real)
    {
    }

    /// Initialize the real and imaginary coefficients.
    GfQuath(GfHalf real, const GfVec3h &imaginary)
        : _imaginary(imaginary), _real(real)
    {
    }

    /// Construct from GfQuatd.
    GF_API
    explicit GfQuath(class GfQuatd const &other);
    /// Construct from GfQuatf.
    GF_API
    explicit GfQuath(class GfQuatf const &other);

    /// Return the identity quaternion, with real coefficient 1 and an
    /// imaginary coefficients all zero.
    static GfQuath GetIdentity() { return GfQuath(1.0); }

    /// Return the real coefficient.
    GfHalf GetReal() const { return _real; }

    /// Set the real coefficient.
    void SetReal(GfHalf real) { _real = real; }

    /// Return the imaginary coefficient.
    const GfVec3h &GetImaginary() const { return _imaginary; }

    /// Set the imaginary coefficients.
    void SetImaginary(const GfVec3h &imaginary) {
        _imaginary = imaginary;
    }

    /// Set the imaginary coefficients.
    void SetImaginary(GfHalf i, GfHalf j, GfHalf k) {
        _imaginary.Set(i, j, k);
    }

    /// Return geometric length of this quaternion.
    GfHalf GetLength() const { return GfSqrt(_GetLengthSquared()); }

    /// length of this quaternion is smaller than \p eps, return the identity
    /// quaternion.
    GfQuath
    GetNormalized(GfHalf eps = GF_MIN_VECTOR_LENGTH) const {
        GfQuath ret(*this);
        ret.Normalize(eps);
        return ret;
    }

    /// Normalizes this quaternion in place to unit length, returning the
    /// length before normalization. If the length of this quaternion is
    /// smaller than \p eps, this sets the quaternion to identity.
    GF_API
    GfHalf Normalize(GfHalf eps = GF_MIN_VECTOR_LENGTH);

    /// Return this quaternion's conjugate, which is the quaternion with the
    /// same real coefficient and negated imaginary coefficients.
    GfQuath GetConjugate() const {
        return GfQuath(GetReal(), -GetImaginary());
    }

    /// Return this quaternion's inverse, or reciprocal.  This is the
    /// quaternion's conjugate divided by it's squared length.
    GfQuath GetInverse() const {
        return GetConjugate() / _GetLengthSquared();
    }

    /// Transform the GfVec3h point. If the quaternion is normalized,
    /// the transformation is a rotation. Given a GfQuath q, q.Transform(point)
    /// is equivalent to:
    ///
    ///     (q * GfQuath(0, point) * q.GetInverse()).GetImaginary()
    ///
    /// but is more efficient.
    GF_API
    GfVec3h Transform(const GfVec3h& point) const;

    /// Hash.
    friend inline size_t hash_value(const GfQuath &q) {
        size_t h = boost::hash<ScalarType>()(q.GetReal());
        boost::hash_combine(h, q.GetImaginary());
        return h;
    }

    /// Component-wise negation.
    GfQuath operator-() const {
        return GfQuath(-GetReal(), -GetImaginary());
    }

    /// Component-wise quaternion equality test. The real and imaginary parts
    /// must match exactly for quaternions to be considered equal.
    bool operator==(const GfQuath &q) const {
        return (GetReal() == q.GetReal() &&
                GetImaginary() == q.GetImaginary());
    }

    /// Component-wise quaternion inequality test. The real and imaginary
    /// parts must match exactly for quaternions to be considered equal.
    bool operator!=(const GfQuath &q) const {
        return !(*this == q);
    }

    /// Post-multiply quaternion \p q into this quaternion.
    GF_API
    GfQuath &operator *=(const GfQuath &q);

    /// Multiply this quaternion's coefficients by \p s.
    GfQuath &operator *=(GfHalf s) {
        _real *= s;
        _imaginary *= s;
        return *this;
    }

    /// Divide this quaternion's coefficients by \p s.
    GfQuath &operator /=(GfHalf s) {
        _real /= s;
        _imaginary /= s;
        return *this;
    }

    /// Add quaternion \p q to this quaternion.
    GfQuath &operator +=(const GfQuath &q) {
        _real += q._real;
        _imaginary += q._imaginary;
        return *this;
    }

    /// Component-wise unary difference operator.
    GfQuath &operator -=(const GfQuath &q)  {
        _real -= q._real;
        _imaginary -= q._imaginary;
        return *this;
    }

    /// Component-wise binary sum operator.
    friend GfQuath
    operator +(const GfQuath &q1, const GfQuath &q2) {
        return GfQuath(q1) += q2;
    }

    /// Component-wise binary difference operator.
    friend GfQuath
    operator -(const GfQuath &q1, const GfQuath &q2) {
        return GfQuath(q1) -= q2;
    }

    /// Returns the product of quaternions \p q1 and \p q2.
    friend GfQuath
    operator *(const GfQuath &q1, const GfQuath &q2) {
        return GfQuath(q1) *= q2;
    }

    /// Returns the product of quaternion \p q and scalar \p s.
    friend GfQuath
    operator *(const GfQuath &q, GfHalf s) {
        return GfQuath(q) *= s;
    }

    /// Returns the product of quaternion \p q and scalar \p s.
    friend GfQuath
    operator *(GfHalf s, const GfQuath &q) {
        return GfQuath(q) *= s;
    }

    /// Returns the product of quaternion \p q and scalar 1 / \p s.
    friend GfQuath
    operator /(const GfQuath &q, GfHalf s) {
        return GfQuath(q) /= s;
    }

  private:
    /// Imaginary part
    GfVec3h _imaginary;

    /// Real part
    GfHalf _real;

    /// Returns the square of the length
    GfHalf
    _GetLengthSquared() const {
        return GfDot(*this, *this);
    }
};

/// Spherically linearly interpolate between \p q0 and \p q1.
///
/// If the interpolant \p alpha is zero, then the result is \p q0, while
/// \p alpha of one yields \p q1.
GF_API GfQuath
GfSlerp(double alpha, const GfQuath& q0, const GfQuath& q1);

GF_API GfQuath
GfSlerp(const GfQuath& q0, const GfQuath& q1, double alpha);

inline GfHalf
GfDot(GfQuath const &q1, GfQuath const &q2) {
    return GfDot(q1.GetImaginary(), q2.GetImaginary()) +
                 q1.GetReal()*q2.GetReal();
}

/// Output a GfQuatd using the format (re, i, j, k)
/// \ingroup group_gf_DebuggingOutput
GF_API std::ostream& operator<<(std::ostream &, GfQuath const &);

PXR_NAMESPACE_CLOSE_SCOPE

#endif // PXR_BASE_GF_QUATH_H
