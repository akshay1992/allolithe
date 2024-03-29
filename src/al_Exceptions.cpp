#include "allolithe/al_Exceptions.hpp"

namespace al{ 

std::ostringstream ModuleNotRegisteredException::message;
std::ostringstream PatchingException::message;

std::ostringstream NodeNotFoundException::message;
std::ostringstream ParametersNotInstantiatedException::message;

}; // namespace al