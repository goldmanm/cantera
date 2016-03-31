//! @file IdealGasReactor.h

// Copyright 2001  California Institute of Technology

#ifndef CT_IDEALGASREACTOR_H
#define CT_IDEALGASREACTOR_H

#include "Reactor.h"

namespace Cantera
{

/**
 * Class IdealGasReactor is a class for stirred reactors that is specifically
 * optimized for ideal gases. In this formulation, temperature replaces the
 * total internal energy as a state variable.
 */
class IdealGasReactor : public Reactor
{
public:
    IdealGasReactor() {}

    virtual int type() const {
        return IdealGasReactorType;
    }

    virtual void setThermoMgr(ThermoPhase& thermo);

    //! @deprecated Use getState instead. To be removed after Cantera 2.3.
    virtual void getInitialConditions(doublereal t0, size_t leny,
                                      doublereal* y);
    virtual void getState(doublereal* y);

    virtual void initialize(doublereal t0 = 0.0);

    virtual void evalEqs(doublereal t, doublereal* y,
                         doublereal* ydot, doublereal* params);

    // Returns a vector with components: [0] is temperature (K), [1] is
    // pressure (pascals), [2] is total number of moles (kmol), and [3...k+3]
    // are the mole fractions of species. Wall species are not not supported
    // currently. the array x should have at least 3+num_species
    virtual void getIntrinsicState(doublereal* x);
    virtual void evalIntrinsicEqns(doublereal time, doublereal* x, doublereal* xdot);

    virtual void updateState(doublereal* y);

    //! Return the index in the solution vector for this reactor of the
    //! component named *nm*. Possible values for *nm* are "mass",
    //! "volume", "temperature", the name of a homogeneous phase species, or the
    //! name of a surface species.
    virtual size_t componentIndex(const std::string& nm) const;

protected:
    vector_fp m_uk; //!< Species molar internal energies
};

}

#endif
