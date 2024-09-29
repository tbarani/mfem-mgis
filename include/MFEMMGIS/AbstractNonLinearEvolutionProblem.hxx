/*!
 * \file   include/MFEMMGIS/AbstractNonLinearEvolutionProblem.hxx
 * \brief
 * \author Thomas Helfer
 * \date   23/03/2021
 */

#ifndef LIB_MFEMMGIS_ABSTRACTNONLINEAREVOLUTIONPROBLEM_HXX
#define LIB_MFEMMGIS_ABSTRACTNONLINEAREVOLUTIONPROBLEM_HXX

#include <map>
#include <string>
#include <memory>
#include <functional>
#include "MFEMMGIS/Config.hxx"
#include "MFEMMGIS/NonLinearResolutionOutput.hxx"

namespace mfem_mgis {

  // forward declarations
  struct Parameter;
  struct Parameters;
  struct FiniteElementDiscretization;
  struct DirichletBoundaryCondition;
  struct BehaviourIntegrator;
  struct Material;
  struct AbstractBoundaryCondition;

  /*!
   * \brief class for solving non linear evolution problems
   */
  struct MFEM_MGIS_EXPORT AbstractNonLinearEvolutionProblem {
    static const char *const SolverVerbosityLevel;
    static const char *const SolverRelativeTolerance;
    static const char *const SolverAbsoluteTolerance;
    static const char *const SolverMaximumNumberOfIterations;
    //! \return the underlying finite element discretization
    virtual FiniteElementDiscretization &getFiniteElementDiscretization() = 0;
    //! \return the underlying finite element discretization
    virtual const FiniteElementDiscretization &getFiniteElementDiscretization()
        const = 0;
    //! \return the underlying finite element discretization
    virtual std::shared_ptr<FiniteElementDiscretization>
    getFiniteElementDiscretizationPointer() = 0;
    //! \return the unknowns at the beginning of the time step
    virtual mfem::Vector &getUnknownsAtBeginningOfTheTimeStep() = 0;
    //! \return the unknowns at the beginning of the time step
    virtual const mfem::Vector &getUnknownsAtBeginningOfTheTimeStep() const = 0;
    //! \return the unknowns at the end of the time step
    virtual mfem::Vector &getUnknownsAtEndOfTheTimeStep() = 0;
    //! \return the unknowns at the end of the time step
    virtual const mfem::Vector &getUnknownsAtEndOfTheTimeStep() const = 0;
    /*!
     * \brief set the solver parameters
     * \param[in] params: parameters
     *
     * The following parameters are allowed:
     *
     * -
     */
    virtual void setSolverParameters(const Parameters &) = 0;
    /*!
     * \brief set the solver parameters
     * \param[in] n: name of the linear solver
     * \param[in] params: parameters
     *
     * The following parameters are allowed:
     *
     * -
     */
    virtual void setLinearSolver(std::string_view, const Parameters &) = 0;
    /*!
     * \brief solve the non linear problem over the given time step
     * \param[in] t: time at the beginning of the time step
     * \param[in] dt: time increment
     */
    virtual NonLinearResolutionOutput solve(const real, const real) = 0;
    /*!
     * \brief add a new behaviour integrator
     * \param[in] n: name of the behaviour integrator
     * \param[in] m: material ids
     * \param[in] l: library name
     * \param[in] b: behaviour name
     */
    virtual void addBehaviourIntegrator(const std::string &,
                                        const Parameter &,
                                        const std::string &,
                                        const std::string &) = 0;
    /*!
     * \brief set material names
     * \param[in] ids: mapping between mesh identifiers and names
     */
    virtual void setMaterialsNames(
        const std::map<size_type, std::string> &) = 0;
    /*!
     * \brief set material names
     * \param[in] ids: mapping between mesh identifiers and names
     */
    virtual void setBoundariesNames(
        const std::map<size_type, std::string> &) = 0;
    /*!
     * \return the list of material identifiers for which a behaviour
     * integrator has been defined.
     */
    virtual std::vector<size_type> getAssignedMaterialsIdentifiers() const = 0;
    /*!
     * \return the material identifier by the given parameter.
     * \note The parameter may hold an integer or a string.
     */
    virtual size_type getMaterialIdentifier(const Parameter &) const = 0;
    /*!
     * \return the material identifier by the given parameter.
     * \note The parameter may hold an integer or a string.
     */
    virtual size_type getBoundaryIdentifier(const Parameter &) const = 0;
    /*!
     * \return the list of materials identifiers described by the given
     * parameter.
     *
     * \note The parameter may hold:
     *
     * - an integer
     * - a string
     * - a vector of parameters which must be either strings and integers.
     *
     * Integers are directly intepreted as materials identifiers.
     *
     * Strings are intepreted as regular expressions which allows the selection
     * of materials by names.
     */
    virtual std::vector<size_type> getMaterialsIdentifiers(
        const Parameter &) const = 0;
    /*!
     * \return the list of boundaries identifiers described by the given
     * parameter.
     *
     * \note The parameter may hold:
     *
     * - an integer
     * - a string
     * - a vector of parameters which must be either strings and integers.
     *
     * Integers are directly intepreted as boundaries identifiers.
     *
     * Strings are intepreted as regular expressions which allows the selection
     * of boundaries by names.
     */
    virtual std::vector<size_type> getBoundariesIdentifiers(
        const Parameter &) const = 0;
    /*!
     * \return the material with the given id
     * \param[in] m: material id
     */
    virtual const Material &getMaterial(const Parameter &) const = 0;
    /*!
     * \return the material with the given id
     * \param[in] m: material id
     */
    virtual Material &getMaterial(const Parameter &) = 0;
    /*!
     * \return the behaviour integrator with the given material id
     * \param[in] m: material id
     */
    virtual const BehaviourIntegrator &getBehaviourIntegrator(
        const size_type) const = 0;
    /*!
     * \return the behaviour integrator with the given material id
     * \param[in] m: material id
     */
    virtual BehaviourIntegrator &getBehaviourIntegrator(const size_type) = 0;
    /*!
     * \brief add a boundary condition
     * \param[in] f: boundary condition
     */
    virtual void addBoundaryCondition(
        std::unique_ptr<AbstractBoundaryCondition>) = 0;
    /*!
     * \brief add a Dirichlet boundary condition
     * \param[in] bc: boundary condition
     */
    virtual void addBoundaryCondition(
        std::unique_ptr<DirichletBoundaryCondition>) = 0;
    /*!
     * \brief add a new post-processing
     * \param[in] p: post-processing
     */
    virtual void addPostProcessing(
        const std::function<void(const real, const real)> &) = 0;
    /*!
     * \brief add a new post-processing
     * \param[in] n: name of the post-processing
     * \param[in] p: parameters
     */
    virtual void addPostProcessing(std::string_view, const Parameters &) = 0;
    /*!
     * \brief execute the registred postprocessings
     * \param[in] t: time at the beginning of the time step
     * \param[in] dt: time increment
     */
    virtual void executePostProcessings(const real, const real) = 0;
    //! \brief revert the state to the beginning of the time step.
    virtual void revert() = 0;
    //! \brief update the state to the end of the time step.
    virtual void update() = 0;
    //! \brief destructor
    virtual ~AbstractNonLinearEvolutionProblem();
  };  // end of struct AbstractNonLinearEvolutionProblem

  /*!
   * \return the material identifier from the parameters from the `Material`
   * parameter.
   *
   * The `Material` parameter must be either a string and an integer.
   *
   * \param[in] p: non linear problem
   * \param[in] params: parameters
   */
  size_type getMaterialIdentifier(const AbstractNonLinearEvolutionProblem &,
                                  const Parameters &);

  /*!
   * \return the boundary identifier from the parameters from the `Boundary`
   * parameter.
   *
   * The `Boundary` parameter must be either a string and an integer.
   *
   * \param[in] p: non linear problem
   * \param[in] params: parameters
   */
  size_type getBoundaryIdentifier(const AbstractNonLinearEvolutionProblem &,
                                  const Parameters &);

  /*!
   * \return the materials identifiers from the parameters if the one of
   * `Material` or `Materials` parameters exist.  If not such parameter exist,
   * all the materials identifiers are returned if `b` is true, or an error is
   * raised.
   *
   * The `Material` parameter must be either a string and an integer.
   * The `Materials` parameter must be either a string, an integer or a vector
   * of parameters which must be either strings or integers.
   *
   * \note `Material` and `Materials` can't be specificed at the same time.
   *
   * \param[in] p: non linear problem
   * \param[in] params: parameters
   * \param[in] b: allowing missing `Material` or `Materials` parameters
   */
  std::vector<size_type> getMaterialsIdentifiers(
      const AbstractNonLinearEvolutionProblem &,
      const Parameters &,
      const bool = true);

  /*!
   * \return the boundaries identifiers from the parameters if the one of
   * `Boundary` or `Boundaries` parameters exist. If not such parameter exist,
   * all the boundaries identifiers are returned if `b` is true, or an error is
   * raised.
   *
   * The `Boundary` parameter must be either a string and an integer.
   * The `Boundaries` parameter must be either a string, an integer or a
   * vector of parameters which must be either strings or integers.
   *
   * \note `Boundary` and `Boundaries` can't be specificed at the same time.
   *
   * \param[in] p: non linear problem
   * \param[in] params: parameters
   * \param[in] b: allowing missing `Boundary` or `Boundaries` parameters
   */
  std::vector<size_type> getBoundariesIdentifiers(
      const AbstractNonLinearEvolutionProblem &,
      const Parameters &,
      const bool = true);

}  // end of namespace mfem_mgis

#endif /* LIB_MFEMMGIS_ABSTRACTNONLINEAREVOLUTIONPROBLEM_HXX */
