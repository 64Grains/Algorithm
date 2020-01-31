/***************************************************************************************************
 Created on: 2020-01-31
 Created by: dquan2007@163.com
 Copyright (c) 2020-2020

 This file is part of C++ NeatSpline Library.

 This library is free software; you can redistribute it and/or modify it under the terms of the GNU
 Lesser General Public License version 3 as published by the Free Software Foundation, with special
 exception defined in the file LICENSE.

***************************************************************************************************/
#ifndef __NURBS_INTERP_H_20200131__
#define __NURBS_INTERP_H_20200131__

/**
 * This file defines the interpolation for nurbs node.
 */
namespace neat
{
    template<typename TYPE>
    class CNurbsInterp
    {
    public:
        CNurbsInterp();
        ~CNurbsInterp();

        // Set start tangent vector
        void SetStartTangent(const TYPE& nxStartTangent_);

        // Set end tangent vector
        void SetEndTangent(const TYPE& nxEndTangent_);

        // Set knot vector
        void SetKnotVector(const VECDOUBLE& vecKnots_);

        // NURBS curve interpolation
        bool InterpNurbsCurve(const std::vector<TYPE>& vecFitPoints_, NurbsNode<TYPE>& NurbsNode_);

    private:
        // Generate node vector by parameterization
        void GenerateKnotVector(const std::vector<TYPE>& vecFitPoints_, VECDOUBLE& vecKnots_);

        // Get the inverse calculation control coefficient matrix
        void GetCoefficientMatrix(const std::vector<TYPE>& vecFitPoints_, const VECDOUBLE& vecKnots_);

        // Solve control points by chasing
        bool ResolveEquationByChase(const std::vector<TYPE>& vecFitPoints_, std::vector<TYPE>& vecPoles_);

        // Calculate the value of the B-spline basis function at the knot
        double GetBSplineBaseValue(const VECDOUBLE& vecKnots_, int nIndex_, int nNumber_);

        // Get the length between two points
        double GetLength(const TYPE& ptLeft_, const TYPE& ptRight_) const;

    private:
        std::vector<TYPE> m_vecRFactor;		// Result vector of fit points
        VECDOUBLE m_vecLFactor;				// Coefficient matrix lower triangle corresponding coefficient
        VECDOUBLE m_vecMFactor;				// Coefficient matrix diagonal corresponding coefficient
        VECDOUBLE m_vecUFactor;				// Coefficient matrix upper triangle corresponding coefficient
        VECDOUBLE m_vecKnots;				// Knot vector
        TYPE m_nxStartTangent;				// Start tangent vector
        TYPE m_nxEndTangent;				// End tangent vector
        bool m_bSetKnotVector;				// Whether to set the knot vector
        bool m_bSetStartTangent;			// Whether to set the start tangent vector
        bool m_bSetEndTangent;				// Whether to set the end tangent vector
        const static int ms_nDegree = 3;	// Degree of non-rational B-spline curve
    };

    /**
     * The implementation of template class.
     */
    template<typename TYPE>
    CNurbsInterp<TYPE>::CNurbsInterp()
        : m_bSetKnotVector(false)
        , m_bSetStartTangent(false)
        , m_bSetEndTangent(false)
    {
    }

    template<typename TYPE>
    CNurbsInterp<TYPE>::~CNurbsInterp()
    {
    }

    template<typename TYPE>
    void CNurbsInterp<TYPE>::SetStartTangent(const TYPE& nxStartTangent_)
    {
        m_bSetStartTangent = true;
        m_nxStartTangent = nxStartTangent_;
    }

    template<typename TYPE>
    void CNurbsInterp<TYPE>::SetEndTangent(const TYPE& nxEndTangent_)
    {
        m_bSetEndTangent = true;
        m_nxEndTangent = nxEndTangent_;
    }

    template<typename TYPE>
    void CNurbsInterp<TYPE>::SetKnotVector(const VECDOUBLE& vecKnots_)
    {
        m_bSetKnotVector = true;
        m_vecKnots = vecKnots_;
    }

    template<typename TYPE>
    bool CNurbsInterp<TYPE>::InterpNurbsCurve(const std::vector<TYPE>& vecFitPoints_, NurbsNode<TYPE>& NurbsNode_)
    {
        // degree of NURBS curve
        NurbsNode_.nDegree = ms_nDegree;

        // generate knot vector
        GenerateKnotVector(vecFitPoints_, NurbsNode_.vecKnots);

        // Construct the coefficient matrix: the coefficient matrix is band-shaped, and the bandwidth is not greater than 3,
        // which can be solved by chasing method
        GetCoefficientMatrix(vecFitPoints_, NurbsNode_.vecKnots);

        // chase method solve control point
        bool _bSuccess = ResolveEquationByChase(vecFitPoints_, NurbsNode_.vecPoles);
        return _bSuccess;
    }

    template<typename TYPE>
    void CNurbsInterp<TYPE>::GenerateKnotVector(const std::vector<TYPE>& vecFitPoints_, VECDOUBLE& vecKnots_)
    {
        NEAT_RAISE_IF(vecFitPoints_.empty(), g_strNurbsParamsError);
        if (m_bSetKnotVector) {
            vecKnots_ = m_vecKnots;
            return;
        }

        // construct the knot vector
        int _nSize = static_cast<int>(vecFitPoints_.size());

        VECDOUBLE _vecChordLength;
        _vecChordLength.reserve(_nSize);
        _vecChordLength.push_back(0.0);

        // Accumulated chord length parameterization method
        for (int i = 1; i < _nSize; ++i) {
            double _nLength = GetLength(vecFitPoints_[i], vecFitPoints_[i - 1]);
            _vecChordLength.push_back(_vecChordLength.back() + _nLength);
        }

        // normalize
        for (int i = 1; i < _nSize - 1; ++i) {
            _vecChordLength[i] /= _vecChordLength.back();
        }

        _vecChordLength.back() = 1.0;

        // knot vector: repeated knot
        vecKnots_.clear();
        vecKnots_.reserve(_nSize + 2 * ms_nDegree);
        for (int i = 0; i < ms_nDegree; ++i) {
            vecKnots_.push_back(0.0);
        }

        for (int i = 0; i < _nSize; ++i) {
            vecKnots_.push_back(_vecChordLength[i]);
        }

        for (int i = 0; i < ms_nDegree; ++i) {
            vecKnots_.push_back(_vecChordLength.back());
        }
    }

    template<typename TYPE>
    void CNurbsInterp<TYPE>::GetCoefficientMatrix(const std::vector<TYPE>& vecFitPoints_, const VECDOUBLE& vecKnots_)
    {
        NEAT_RAISE(vecKnots_.size() != vecFitPoints_.size() + ms_nDegree + 3, g_strNurbsParamsError);

        // fit points
        int _nFitPointSize = static_cast<int>(vecFitPoints_.size());
        int _nKnotSize = static_cast<int>(vecKnots_.size());

        // initialization
        m_vecLFactor = VECDOUBLE(_nFitPointSize, 0.0);
        m_vecMFactor = VECDOUBLE(_nFitPointSize, 0.0);
        m_vecUFactor = VECDOUBLE(_nFitPointSize, 0.0);
        m_vecRFactor = vecFitPoints_;

        // matrix coefficient
        int _nIndex = 1;
        for (int i = ms_nDegree + 1, _nSize = _nKnotSize - ms_nDegree - 1; i < _nSize; ++i) {
            m_vecLFactor[_nIndex] = GetBSplineBaseValue(vecKnots_, i, 1);
            m_vecMFactor[_nIndex] = GetBSplineBaseValue(vecKnots_, i, 2);
            m_vecUFactor[_nIndex] = GetBSplineBaseValue(vecKnots_, i, 3);
            ++_nIndex;
        }

        // starting condition
        if (m_bSetStartTangent) {
            // given start tangent vector
            m_vecMFactor[0] = 3.0 / (vecKnots_[ms_nDegree + 1] - vecKnots_[ms_nDegree]);
            m_vecRFactor[0] = m_nxStartTangent + 3.0 / (vecKnots_[ms_nDegree + 1] - vecKnots_[ms_nDegree]) * vecFitPoints_[0];
        }
        else {
            // free endpoint
            m_vecMFactor[0] = 6.0 / pow(vecKnots_[ms_nDegree + 1] - vecKnots_[ms_nDegree], 2)
                + 6.0 / ((vecKnots_[ms_nDegree + 1] - vecKnots_[ms_nDegree]) * (vecKnots_[ms_nDegree + 2] - vecKnots_[ms_nDegree]));
            m_vecUFactor[0] = -6.0 / ((vecKnots_[ms_nDegree + 1] - vecKnots_[ms_nDegree]) * (vecKnots_[ms_nDegree + 2] - vecKnots_[ms_nDegree]));
            m_vecRFactor[0] = 6.0 / pow(vecKnots_[ms_nDegree + 1] - vecKnots_[ms_nDegree], 2) * vecFitPoints_[0];
        }

        // end condition
        int _nLength = _nKnotSize - ms_nDegree - 1;
        if (m_bSetEndTangent) {
            // given end tangent vector
            m_vecMFactor[_nFitPointSize - 1] = 3.0 / (vecKnots_[_nLength] - vecKnots_[_nLength - 1]);
            m_vecRFactor[_nFitPointSize - 1] = 3.0 / (vecKnots_[_nLength] - vecKnots_[_nLength - 1])
                * vecFitPoints_[_nFitPointSize - 1] - m_nxEndTangent;
        }
        else {
            // free endpoint
            m_vecLFactor[_nFitPointSize - 1] = -6.0 / ((vecKnots_[_nLength] - vecKnots_[_nLength - 1])
                * (vecKnots_[_nLength] - vecKnots_[_nLength - 2]));
            m_vecMFactor[_nFitPointSize - 1] = 6.0 / ((vecKnots_[_nLength] - vecKnots_[_nLength - 1])
                * (vecKnots_[_nLength] - vecKnots_[_nLength - 2])) + 6.0 / pow(vecKnots_[_nLength] - vecKnots_[_nLength - 1], 2);
            m_vecRFactor[_nFitPointSize - 1] = 6.0 / pow(vecKnots_[_nLength] - vecKnots_[_nLength - 1], 2) * vecFitPoints_[_nFitPointSize - 1];
        }
    }

    template<typename TYPE>
    bool CNurbsInterp<TYPE>::ResolveEquationByChase(const std::vector<TYPE>& vecFitPoints_, std::vector<TYPE>& vecPoles_)
    {
        // number of rows in the matrix
        int _nSize = static_cast<int>(m_vecMFactor.size());

        VECDOUBLE _vecLMatrix(_nSize, 0.0);
        VECDOUBLE _vecUMatrix(_nSize, 0.0);

        // LU factorization of coefficient matrix A: A = LU
        _vecUMatrix[0] = m_vecMFactor[0];
        for (int i = 1; i < _nSize; ++i) {
            if (DOUBLE_EQU_ZERO(_vecUMatrix[i - 1])) {
                // coefficient matrix does not support LU factorization
                NEAT_RAISE_IF(true, g_strUnknownError);
                return false;
            }

            _vecLMatrix[i] = m_vecLFactor[i] / _vecUMatrix[i - 1];
            _vecUMatrix[i] = m_vecMFactor[i] - _vecLMatrix[i] * m_vecUFactor[i - 1];
        }

        if (DOUBLE_EQU_ZERO(_vecUMatrix[_nSize - 1])) {
            NEAT_RAISE_IF(true, g_strUnknownError);
            return false;
        }

        // Ax=b => LUx=b => Ux=y,Ly=b
        // Solve: Ly=b
        std::vector<TYPE> _vecRy = m_vecRFactor;
        for (int i = 1; i < _nSize; ++i) {
            _vecRy[i] = m_vecRFactor[i] - _vecLMatrix[i] * _vecRy[i - 1];
        }

        // Solve: Ux = y
        std::vector<TYPE> _vecRx = _vecRy;
        _vecRx[_nSize - 1] = _vecRy[_nSize - 1] / _vecUMatrix[_nSize - 1];
        for (int i = (int)_nSize - 2; i >= 0; --i) {
            _vecRx[i] = (_vecRy[i] - m_vecUFactor[i] * _vecRx[i + 1]) / _vecUMatrix[i];
        }

        // Control point
        vecPoles_.clear();
        vecPoles_.reserve(_nSize + 2);
        vecPoles_.push_back(vecFitPoints_.front());
        for (int i = 0; i < _nSize; ++i) {
            vecPoles_.push_back(_vecRx[i]);
        }

        vecPoles_.push_back(vecFitPoints_.back());

        return true;
    }

    template<typename TYPE>
    double CNurbsInterp<TYPE>::GetBSplineBaseValue(const VECDOUBLE& vecKnots_, int nIndex_, int nNumber_)
    {
        // Calculate the value of the B-spline basis function at the knot
        double _nBaseValue = 0.0;

        // Calculate directly according to the formula
        switch (nNumber_)
        {
        case 1:
            _nBaseValue = pow(vecKnots_[nIndex_ + 1] - vecKnots_[nIndex_], 2) / ((vecKnots_[nIndex_ + 1] - vecKnots_[nIndex_ - 1])
                * (vecKnots_[nIndex_ + 1] - vecKnots_[nIndex_ - 2]));
            break;
        case 2:
            _nBaseValue = (vecKnots_[nIndex_ + 1] - vecKnots_[nIndex_]) * (vecKnots_[nIndex_] - vecKnots_[nIndex_ - 2])
                / ((vecKnots_[nIndex_ + 1] - vecKnots_[nIndex_ - 1]) * (vecKnots_[nIndex_ + 1] - vecKnots_[nIndex_ - 2]))
                + (vecKnots_[nIndex_] - vecKnots_[nIndex_ - 1]) * (vecKnots_[nIndex_ + 2] - vecKnots_[nIndex_])
                / ((vecKnots_[nIndex_ + 1] - vecKnots_[nIndex_ - 1]) * (vecKnots_[nIndex_ + 2] - vecKnots_[nIndex_ - 1]));
            break;
        case 3:
            _nBaseValue = pow(vecKnots_[nIndex_] - vecKnots_[nIndex_ - 1], 2) / ((vecKnots_[nIndex_ + 1] - vecKnots_[nIndex_ - 1])
                * (vecKnots_[nIndex_ + 2] - vecKnots_[nIndex_ - 1]));
            break;
        default:
            break;
        }

        return _nBaseValue;
    }

} // End namespace neat

#endif // __NURBS_INTERP_H_20200131__