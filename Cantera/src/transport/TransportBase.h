/**
 *  @file TransportBase.h  
 *    Headers for the Transport object, which is the virtual base class
 *    for all transport property evaluators and also includes the 
 *    tranprops group definition
 *    (see \ref tranprops and \link Cantera::Transport Transport \endlink) .
 *
 *   Provides class Transport.
 */

/*
 * $Revision$
 * $Date$
 */

// Copyright 2001-2003  California Institute of Technology


/**
 * @defgroup tranprops Transport Properties for Species in Phases
 *
 * @ingroup phases
 *
 * These classes provide transport properties.
 */

#ifndef CT_TRANSPORTBASE_H
#define CT_TRANSPORTBASE_H

#include "ThermoPhase.h"


namespace Cantera {

  class TransportParams;
  class GasTransportParams;
  class LiquidTransportParams;

  /*!
   * \addtogroup tranprops 
   */
  //@{ 

  const int CK_Mode = 10;

  // types of transport models that can be constructed
  const int None                 = 199;
  const int cMulticomponent      = 200;
  const int CK_Multicomponent    = 202;
  const int cMixtureAveraged     = 210;
  const int CK_MixtureAveraged   = 211;
  const int cSolidTransport      = 300;
  const int cDustyGasTransport   = 400;
  const int cUserTransport       = 500;
  const int cFtnTransport        = 600;
  const int cLiquidTransport     = 700;
  const int cAqueousTransport    = 750;
  const int cSimpleTransport     = 770;
  const int cRadiativeTransport  = 800;
  const int cWaterTransport      = 721;
  //@} 

  // forward reference
  class XML_Writer;

  //! The diffusion fluxes must be referenced to a particular reference
  //! fluid velocity.
  /*!
   * Most typical is to reference the diffusion fluxes to the mass averaged velocity, but 
   * referencing to the mole averaged velocity is suitable for some 
   * liquid flows, and referencing to a single species is suitable for 
   * solid phase transport within a lattice.  Currently, the identity of the reference
   * velocity is coded into each transport object as a typedef named VelocityBasis, which
   * is equated to an integer. Negative values of this variable refer to mass or mole-averaged
   * velocities.  Zero or positive quantities refers to the reference
   * velocity being referenced to a particular species. Below are the predefined constants
   * for its value.
   *
   *  - VB_MASSAVG    Diffusion velocities are based on the mass averaged velocity
   *  - VB_MOLEAVG    Diffusion velocities are based on the mole averaged velocities
   *  - VB_SPECIES_0  Diffusion velocities are based on the relative motion wrt species 0
   *  - VB_SPECIES_1  Diffusion velocities are based on the relative motion wrt species 1
   *
   * @ingroup tranprops
   */
  typedef int VelocityBasis;

  /*!
   * \addtogroup tranprops 
   */
  //@{ 
  //! Diffusion velocities are based on the mass averaged velocity
  const VelocityBasis VB_MASSAVG = -1;
  //! Diffusion velocities are based on the mole averaged velocities
  const VelocityBasis VB_MOLEAVG = -2;
  //! Diffusion velocities are based on the relative motion wrt species 0
  const VelocityBasis VB_SPECIES_0 = 0;
  //! Diffusion velocities are based on the relative motion wrt species 1
  const VelocityBasis VB_SPECIES_1 = 1;
  //@}

  //! Base class for transport property managers.
  /*!
   *  All classes that compute transport properties for a single phase
   *  derive from this class.  Class
   *  %Transport is meant to be used as a base class only. It is
   *  possible to instantiate it, but its methods throw exceptions if
   *  called.
   *
   *  Note, transport properties for multiphase situations have yet to be
   *  fully developed within Cantera.
   *
   *  All member functions are virtual, unless otherwise stated.
   *
   * <HR>
   * <H2> Relationship of the %Transport class to the %ThermoPhase Class </H2>
   * <HR>
   *
   *   This section describes how calculations are carried out within
   *   the %Transport class. The %Transport class and derived classes of the
   *   the %Transport class necessarily use the %ThermoPhase class to obtain
   *   the list of species and the thermodynamic state of the phase. 
   *   
   *   No state information is storred within %Transport classes.  
   *   Queries to the underlying ThermoPhase object must be made to obtain
   *   the state of the system.
   *
   *   An exception to this however is the state information concerning the
   *   the gradients of variables. This information is not storred within
   *   the ThermoPhase objects. It may be collected within the Transport objects.
   *   In fact, the meaning of const operations within the Transport class
   *   refers to calculations which do not change the state of the
   *   system nor the state of the first order gradients of the system. 
   *
   *   When a const operation is evoked within the Transport class, it is
   *   also implicitly assumed that the underlying state within the ThermoPhase 
   *   object has not changed its values.
   *
   *   
   * <HR>
   * <H2> Diffusion Fluxes and their Relationship to Reference Velocities </H2>
   * <HR>
   *
   *  The diffusion fluxes must be referenced to a particular reference
   *  fluid velocity.
   *  Most typical is to reference the diffusion fluxes to the mass averaged velocity, but 
   *  referencing to the mole averaged velocity is suitable for some 
   *  liquid flows, and referencing to a single species is suitable for 
   *  solid phase transport within a lattice.  Currently, the identity of the reference
   *  velocity is coded into each transport object as a typedef named VelocityBasis, which
   *  is equated to an integer. Negative values of this variable refer to mass or mole-averaged
   *  velocities.  Zero or positive quantities refers to the reference
   *  velocity being referenced to a particular species. Below are the predefined constants
   *  for its value.
   *
   *  - VB_MASSAVG    Diffusion velocities are based on the mass averaged velocity
   *  - VB_MOLEAVG    Diffusion velocities are based on the mole averaged velocities
   *  - VB_SPECIES_0  Diffusion velocities are based on the relative motion wrt species 0
   *  - VB_SPECIES_1  Diffusion velocities are based on the relative motion wrt species 1
   *
   *  All transport managers specify a default reference velocity in their default constructors.
   *  All gas phase transport managers by default specify the mass-averaged velocity as their
   *  reference velocities.
   *
   *
   *  @todo Provide a general mechanism to store the gradients of state variables
   *        within the system.
   *
   *  @ingroup tranprops
   */
  class Transport {

  public:

    //!  Constructor.
    /*!
     * New transport managers should be created using
     * TransportFactory, not by calling the constructor directly.
     *
     *  @param   thermo   Pointer to the ThermoPhase class representing
     *                    this phase. 
     *  @param   ndim     Dimension of the flux vector used in the calculation.
     *
     * @see TransportFactory
     */
    Transport(thermo_t* thermo=0, int ndim = 1);

    //! Destructor.
    virtual ~Transport();   

    //!  Copy Constructor for the %Transport  object.
    /*!
     * @param right  Transport to be copied
     */
    Transport(const Transport &right);

    //! Assignment operator
    /*!
     *  This is NOT a virtual function.
     *
     * @param right    Reference to Transport object to be copied into the
     *                 current one.
     */
    Transport&  operator=(const Transport& right);

    //! Duplication routine for objects which inherit from
    //! %Transport
    /*!
     *  This virtual routine can be used to duplicate %Transport objects
     *  inherited from %Transport even if the application only has
     *  a pointer to %Transport to work with.
     *
     *  These routines are basically wrappers around the derived copy
     *  constructor.
     */
    // Note ->need working copy constructors and operator=() functions for all first
    virtual Transport *duplMyselfAsTransport() const;

    /**
     * Transport model. The transport model is the set of
     * equations used to compute the transport properties. This
     * virtual method returns an integer flag that identifies the
     * transport model implemented. The base class returns 0.
     */
    virtual int model() const {return 0;}

    /**
     * Phase object. Every transport manager is designed to compute
     * properties for a specific phase of a mixture, which might be a
     * liquid solution, a gas mixture, a surface, etc. This method
     * returns a reference to the object representing the phase
     * itself.
     */        
    thermo_t& thermo() { return *m_thermo; }


    /**
     * Returns true if the transport manager is ready for use.
     */
    bool ready();

    
    //! Returns an integer index number.
    /*!
     *  This is for internal use
     *  of Cantera, and may be removed in the future.
     *
     *  @return  Returns the index number
     *
     *  @deprecated
     */
    int index() const ;

    //! Set an integer index number.
    /*!
     *  This is for internal use of
     *  Cantera, and may be removed in the future.
     *
     *  @param i  index value
     *
     *  @deprecated
     */
    void setIndex(int i);

    //! Set the number of dimensions to be expected in flux expressions
    /*!
     *   Internal memory will be set with this value.
     *
     *  @param ndim  Number of dimensions in flux expressions
     */
    void setNDim(const int ndim);

    //! Return the number of dimensions in flux expressions
    /*!
     *   @return  Returns the number of dimensions
     */
    int nDim() const {
      return m_nDim;
    }

    /**
     * @name Transport Properties
     */
    //@{
         

    /**
     * The viscosity in Pa-s. 
     */
    virtual doublereal viscosity() 
    { return err("viscosity"); }

    //! Returns the pure species viscosities
    /*!
     *  The units are Pa-s and the length is the number of species
     *
     * @param visc   Vector of viscosities
     */
    virtual void getSpeciesViscosities(doublereal* const visc)
    { err("getSpeciesViscosities"); }

    /**
     * The bulk viscosity in Pa-s. The bulk viscosity is only
     * non-zero in rare cases. Most transport managers either
     * overload this method to return zero, or do not implement
     * it, in which case an exception is thrown if called.
     */
    virtual doublereal bulkViscosity()  
    { return err("bulkViscosity"); }

    //!  Returns the mixture thermal conductivity in W/m/K. 
    /*!
     *   Units are in W / m K  or equivalently kg m / s3 K
     *
     * @return returns thermal conductivity in W/m/K.
     */
    virtual doublereal thermalConductivity()
    { return err("thermalConductivity"); }

    /**
     * The electrical conductivity (Siemens/m).
     */
    virtual doublereal electricalConductivity()
    { return err("electricalConductivity"); }

    
    //! Get the Electrical mobilities (m^2/V/s).
    /*!
     *   This function returns the mobilities. In some formulations
     *   this is equal to the normal mobility multiplied by faraday's constant.
     *
     *   Frequently, but not always, the mobility is calculated from the
     *   diffusion coefficient using the Einstein relation
     *
     *     \f[ 
     *          \mu^e_k = \frac{F D_k}{R T}
     *     \f]
     *
     *
     * @param mobil_e  Returns the mobilities of
     *               the species in array \c mobil_e. The array must be
     *               dimensioned at least as large as the number of species.
     */
    virtual void getMobilities(doublereal* const mobil_e)
    { err("getMobilities"); }

    //! Get the fluid mobilities (s kmol/kg).
    /*!
     *   This function returns the fluid mobilities. Usually, you have
     *   to multiply Faraday's constant into the resulting expression
     *   to general a species flux expression.
     *
     *   Frequently, but not always, the mobility is calculated from the
     *   diffusion coefficient using the Einstein relation
     *
     *     \f[ 
     *          \mu^f_k = \frac{D_k}{R T}
     *     \f]
     *
     *
     * @param mobil_f  Returns the mobilities of
     *               the species in array \c mobil. The array must be
     *               dimensioned at least as large as the number of species.
     */
    virtual void getFluidMobilities(doublereal* const mobil_f)
    { err("getFluidMobilities"); }


    //@}
    
    //! Compute the mixture electrical conductivity 
    doublereal getElectricConduct( );
    
    //! Compute the electric current 
    /*!
     * @param ndim The number of spatial dimensions (1, 2, or 3).
     * @param grad_T The temperature gradient (ignored in this model).
     * @param ldx  Leading dimension of the grad_X array.
     * @param grad_X The gradient of the mole fraction
     * @param ldf  Leading dimension of the grad_V and current vectors.
     * @param grad_V The electrostatic potential gradient.
     * @param current The electric current in A/m^2.
     */
    void getElectricCurrent(int ndim, 
			    const doublereal* grad_T, 
			    int ldx, 
			    const doublereal* grad_X, 
			    int ldf, 
			    const doublereal* grad_V, 
			    doublereal* current) ;
    
    //! Get the species diffusive mass fluxes wrt to 
    //! the mass averaged velocity, 
    //! given the gradients in mole fraction and temperature
    /*!
     *  Units for the returned fluxes are kg m-2 s-1.
     * 
     *  @param ndim Number of dimensions in the flux expressions
     *  @param grad_T Gradient of the temperature
     *                 (length = ndim)
     * @param ldx  Leading dimension of the grad_X array 
     *              (usually equal to m_nsp but not always)
     * @param grad_X Gradients of the mole fraction
     *             Flat vector with the m_nsp in the inner loop.
     *             length = ldx * ndim
     * @param ldf  Leading dimension of the fluxes array 
     *              (usually equal to m_nsp but not always)
     * @param fluxes  Output of the diffusive mass fluxes
     *                Flat vector with the m_nsp in the inner loop.
     *                length = ldx * ndim
     */
    virtual void getSpeciesFluxes(int ndim, 
				  const doublereal* grad_T, 
				  int ldx, 
				  const doublereal* grad_X,
				  int ldf, 
				  doublereal* fluxes) { 
      err("getSpeciesFluxes"); 
    }

    //! Get the species diffusive mass fluxes wrt to 
    //! the mass averaged velocity, 
    //! given the gradients in mole fraction, temperature 
    //! and electrostatic potential.
    /*!
     *  Units for the returned fluxes are kg m-2 s-1.
     * 
     *  @param ndim Number of dimensions in the flux expressions
     *  @param grad_T Gradient of the temperature
     *                 (length = ndim)
     * @param ldx  Leading dimension of the grad_X array 
     *              (usually equal to m_nsp but not always)
     * @param grad_X Gradients of the mole fraction
     *             Flat vector with the m_nsp in the inner loop.
     *             length = ldx * ndim
     * @param ldf  Leading dimension of the fluxes array 
     *              (usually equal to m_nsp but not always)
     * @param grad_Phi Gradients of the electrostatic potential
     *                 (length = ndim)
     * @param fluxes  Output of the diffusive mass fluxes
     *             Flat vector with the m_nsp in the inner loop.
     *             length = ldx * ndim
     */
    virtual void getSpeciesFluxesES(int ndim, 
				    const doublereal* grad_T, 
				    int ldx, 
				    const doublereal* grad_X,
				    int ldf, 
				    const doublereal* grad_Phi,
				    doublereal* fluxes) { 
      getSpeciesFluxes( ndim, grad_T, ldx, grad_X, ldf, fluxes );
    }

    
    //! Get the species diffusive velocities wrt to 
    //! the mass averaged velocity, 
    //! given the gradients in mole fraction and temperature
    /*!
     *  Units for the returned velocities are m s-1
     * 
     *  @param ndim Number of dimensions in the flux expressions
     *  @param grad_T Gradient of the temperature
     *                 (length = ndim)
     * @param ldx  Leading dimension of the grad_X array 
     *              (usually equal to m_nsp but not always)
     * @param grad_X Gradients of the mole fraction
     *             Flat vector with the m_nsp in the inner loop.
     *             length = ldx * ndim
     * @param ldf  Leading dimension of the fluxes array 
     *              (usually equal to m_nsp but not always)
     * @param Vdiff  Output of the diffusive velocities wrt the mass-averaged
     *               velocity
     *               Flat vector with the m_nsp in the inner loop.
     *               length = ldx * ndim
     *               units are m / s.
     */
    virtual void getSpeciesVdiff(int ndim, 
				 const doublereal* grad_T, 
				 int ldx, 
				 const doublereal* grad_X,
				 int ldf, 
				 doublereal* Vdiff) { 
      err("getSpeciesVdiff"); 
    }

    //! Get the species diffusive velocities wrt to the mass averaged velocity, 
    //! given the gradients in mole fraction, temperature,
    //! and electrostatic potential.
    /*!
     *  Units for the returned velocities are m s-1.
     * 
     *  @param ndim Number of dimensions in the flux expressions
     *  @param grad_T Gradient of the temperature
     *                 (length = ndim)
     * @param ldx  Leading dimension of the grad_X array 
     *              (usually equal to m_nsp but not always)
     * @param grad_X Gradients of the mole fraction
     *             Flat vector with the m_nsp in the inner loop.
     *             length = ldx * ndim
     * @param ldf  Leading dimension of the fluxes array 
     *              (usually equal to m_nsp but not always)
     * @param grad_Phi Gradients of the electrostatic potential
     *                 (length = ndim)
     * @param Vdiff  Output of the diffusive velocities wrt the mass-averaged velocity
     *               Flat vector with the m_nsp in the inner loop.
     *               length = ldx * ndim
     *               units are m / s.
     */
    virtual void getSpeciesVdiffES(int ndim, 
				   const doublereal* grad_T, 
				   int ldx, 
				   const doublereal* grad_X,
				   int ldf, 
				   const doublereal* grad_Phi,
				   doublereal* Vdiff) { 
      getSpeciesVdiff( ndim, grad_T, ldx, grad_X, ldf, Vdiff );
    }

    
    //! Get the molar fluxes [kmol/m^2/s], given the thermodynamic
    //! state at two nearby points. 
    /*!
     * @param state1 Array of temperature, density, and mass
     *               fractions for state 1.
     * @param state2 Array of temperature, density, and mass
     *               fractions for state 2.  
     * @param delta  Distance from state 1 to state 2 (m).
     * @param cfluxes Output array containing the diffusive molar fluxes of species
     *               from state1 to state2. This is a flat vector with the 
     *               m_nsp in the inner loop.
     *               length = ldx * ndim.
     *               Units are [kmol/m^2/s].
     */ 
    virtual void getMolarFluxes(const doublereal * const state1,
				const doublereal * const state2, const doublereal delta, 
				doublereal * const cfluxes) { 
      err("getMolarFluxes"); 
    }

   
    //!  Get the mass fluxes [kg/m^2/s], given the thermodynamic
    //!  state at two nearby points. 
    /*!
     * @param state1 Array of temperature, density, and mass
     *               fractions for state 1.
     * @param state2 Array of temperature, density, and mass
     *               fractions for state 2.  
     * @param delta Distance from state 1 to state 2 (m).
     * @param mfluxes Output array containing the diffusive mass fluxes of species
     *               from state1 to state2. This is a flat vector with the 
     *               m_nsp in the inner loop.
     *               length = ldx * ndim.
     *               Units are [kg/m^2/s].
     */ 
    virtual void getMassFluxes(const doublereal* state1,
			       const doublereal* state2, doublereal delta, 
			       doublereal* mfluxes) { 
      err("getMassFluxes"); 
    }
    
    //! Return a vector of Thermal diffusion coefficients [kg/m/sec].
    /*!
     * The thermal diffusion coefficient \f$ D^T_k \f$ is defined
     * so that the diffusive mass flux of species k induced by the
     * local temperature gradient is \f[ M_k J_k = -D^T_k \nabla
     * \ln T. \f]. The thermal diffusion coefficient can be either
     * positive or negative.
     * 
     * @param dt On return, dt will contain the species thermal
     *           diffusion coefficients.  Dimension dt at least as large as
     *           the number of species.
     */
    virtual void getThermalDiffCoeffs(doublereal* const dt)  {
      err("getThermalDiffCoeffs"); 
    }


    //! Returns the matrix of binary diffusion coefficients [m^2/s].
    /*!
     *  @param ld  Inner stride for writing the two dimension diffusion
     *             coefficients into a one dimensional vector
     *  @param d   Diffusion coefficient matrix (must be at least m_k * m_k 
     *             in length.
     */ 
    virtual void getBinaryDiffCoeffs(const int ld, doublereal* const d) { 
      err("getBinaryDiffCoeffs"); 
    }

    
    //! Return the Multicomponent diffusion coefficients. Units: [m^2/s]. 
    /*!
     * If the transport manager implements a multicomponent diffusion
     * model, then this method returns the array of multicomponent
     * diffusion coefficients. Otherwise it throws an exception.
     *
     *  @param ld  The dimension of the inner loop of d (usually equal to m_nsp)
     *  @param d  flat vector of diffusion coefficients, fortran ordering.
     *            d[ld*j+i] is the D_ij diffusion coefficient (the diffusion
     *            coefficient for species i due to species j).
     */
    virtual void getMultiDiffCoeffs(const int ld, doublereal* const d) 
    { err("getMultiDiffCoeffs"); }


    //! Returns a vector of mixture averaged diffusion coefficients
    /**
     * Mixture-averaged diffusion coefficients [m^2/s].  If the
     *
     * transport manager implements a mixture-averaged diffusion
     * model, then this method returns the array of
     * mixture-averaged diffusion coefficients. Otherwise it
     * throws an exception.
     *
     * @param d  Return vector of mixture averaged diffusion coefficients
     *           Units = m2/s. Length = n_sp
     */
    virtual void getMixDiffCoeffs(doublereal* const d) 
    { err("getMixDiffCoeffs"); }


    //! Set model parameters for derived classes
    /*!
     *   This method may be
     *   overloaded in subclasses to set model-specific parameters.
     *   The primary use of this class is to set parameters while in the
     *   middle of a calculation.
     *
     *  @param type    Specifies the type of parameters to set
     *                 0 : Diffusion coefficient
     *                 1 : Thermal Conductivity
     *                 The rest are currently unused.
     *  @param k       Species index to set the parameters on
     *  @param p       Vector of parameters. The length of the vector
     *                 varies with the parameterization
     */
    virtual void setParameters(const int type, const int k,
			       const doublereal* const p); 
   
    //! Sets the velocity basis
    /*!
     *   What the transport object does with this parameter is up to the
     *   individual operator. Currently, this is not functional for most 
     *   transport operators including all of the gas-phase operators.
     *
     *   @param ivb   Species the velocity basis
     */
    void setVelocityBasis(VelocityBasis ivb) 
    { 
      m_velocityBasis = ivb; 
    }
    
    //!  Gets the velocity basis
    /*!
     *   What the transport object does with this parameter is up to the
     *   individual operator. Currently, this is not functional for most 
     *   transport operators including all of the gas-phase operators. 
     *
     *   @return   Returns the velocity basis
     */
    VelocityBasis getVelocityBasis( ) const { 
      return m_velocityBasis; 
    }

    friend class TransportFactory;


  protected:

    /**
     * @name Transport manager construction
     * These methods are used internally during construction.  
     * @{
     */

    /**
     * Called by TransportFactory to set parameters.
     */
    //virtual bool init(TransportParams& tr)
    //{ err("init"); return false; }

    
    //! Called by TransportFactory to set parameters.
    /*!
     *  This is called by classes that use the gas phase parameter
     *  list to initialize themselves.
     *
     *   @param tr Reference to the parameter list that will be used
     *             to initialize the class
     */
    virtual bool initGas(GasTransportParams& tr)
    {
      err("initGas");
      return false;
    }

    //! Called by TransportFactory to set parameters.
    /*!
     *  This is called by classes that use the liquid phase parameter
     *  list to initialize themselves.
     *
     *   @param tr Reference to the parameter list that will be used
     *             to initialize the class
     */
    virtual bool initLiquid(LiquidTransportParams& tr)
    { 
      err("initLiquid"); 
      return false; 
    }
    
    //! Specifies the %ThermPhase object. 
    /*!
     *   @param   thermo  Reference to the ThermoPhase object that
     *                    the transport object will use
     */ 
    void setThermo(thermo_t& thermo);

    
    //! Enable the transport object for use.
    /*!
     * Once finalize() has been called, the
     * transport manager should be ready to compute any supported
     * transport property, and no further modifications to the
     * model parameters should be made.
     */
    void finalize();

    //@}

    //!  pointer to the object representing the phase 
    thermo_t*  m_thermo;

    //!  true if finalize has been called
    bool      m_ready;

    //! Number of species
    size_t    m_nmin;

    //! Value of the internal index
    /*!
     *  @deprecated
     */
    int       m_index;

    //! Number of dimensions used in flux expresions
    int       m_nDim;

    //!    Velocity basis from which diffusion velocities are computed. 
    //!    Defaults to the mass averaged basis = -2
    int m_velocityBasis;

  private:

    //! Error routine
    /*!
     * Throw an exception if a method of this class is
     * invoked. This probably indicates that a transport manager
     * is being used that does not implement all virtual methods,
     * and one of those methods was called by the application
     * program. For example, a transport manager that computes the
     * thermal conductivity of a solid may not define the
     * viscosity() method, since the viscosity is in this case
     * meaningless. If the application invokes the viscosity()
     * method, the base class method will be called, resulting in
     * an exception being thrown.
     *
     *  @param msg  Descriptive message string to add to the error report
     *
     *  @return  returns a double, though we will never get there
     */
    doublereal err(std::string msg) const;

  };

  //! General definition for the transport class
  /*!
   * \ingroups tranprops
   */
  typedef Transport transport_t;

}

#endif
