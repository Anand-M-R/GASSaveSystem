#include "Modules/ModuleManager.h"

class FGASSaveSystemModule : public IModuleInterface
{
public:
	virtual void StartupModule() override {}
	virtual void ShutdownModule() override {}
};

IMPLEMENT_MODULE(FGASSaveSystemModule, GASSaveSystem)
