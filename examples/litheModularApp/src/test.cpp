#include "include_modules.hpp"

namespace al{

std::string StereoSink::moduleName = "StereoSink";
int StereoSink::moduleID = al::RegisterModule<StereoSink>(StereoSink::moduleName);


}; // namespace al

std::string Oscillator::moduleName = "Oscillator";
int Oscillator::moduleID = al::RegisterModule<Oscillator>(Oscillator::moduleName);

