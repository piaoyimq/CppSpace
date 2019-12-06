/*
 * CollectionSkills.cpp
 *
 *  Created on: Dec 5, 2019
 *      Author: azhweib
 */





/*
* Queries whether the current event is a specified "T" type.
* The T type can be one of the following values: VariableEvent, DatalogEvent, TestflowEvent, TestProgramEvent, AlarmEvent,
*                                                SessionEvent, RecipeEvent, WorkspaceEvent, PHEvent and LicenseEvent.
*
* <h4>Example</h4>
* In this example, event is an invoked Event instance.
*
* \code
* if(event.hasExtension<DatalogEvent>()){
*       DatalogEvent datalogEvent = event.getExtension<DatalogEvent>();
* }
* \endcode
*
* @return true if the event is an instance of T. Otherwise, it returns false.
*
*/
template<class T>
bool hasExtension() const
{
return dynamic_cast<const T*>(this) != 0;
}


/***
 * Retrieves the converted instance of T.
 * The T can be one of the following values: VariableEvent, DatalogEvent, TestflowEvent, TestProgramEvent,
 *                                           AlarmEvent, SessionEvent, RecipeEvent, WorkspaceEvent, PHEvent and LicenseEvent.
 *
 * <h4>Example</h4>
 * In this example, event is an invoked Event instance.
 *
 * \code
 * if(event.hasExtension<DatalogEvent>()){
 *       DatalogEvent datalogEvent = event.getExtension<DatalogEvent>();
 * }
 * \endcode
 *
 * @return The converted instance of T.
 * @see hasExtension()
 *
 */
template<class T>
const T* getExtension() const
{
  if (hasExtension< const T>())
    return dynamic_cast<const T*>(this);
  std::bad_cast e;
  throw e;
}
