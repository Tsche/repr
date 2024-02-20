#pragma once
#include <librepr/customization/enum.h>
#include <librepr/customization/reflection.h>

namespace librepr {

/** This class is used to control a lot of librepr's behavior. 
 * Please refer to @ref md_docs_2customization for an overview of how this can be used.
 * @tparam T Type to control behavior for.
 */
template <typename T>
struct Settings;

} // namespace librepr
