#ifndef RENDERMANAGER_H
#define RENDERMANAGER_H

class RenderManager
{

public:
	static RenderManager* Instance();
	void Init();
	
private:
	RenderManager(void);
	~RenderManager(void);

	void GeometryPass();
	void StencilPass();
	void LightPass();

	void CalcMS();

	static RenderManager* pInstance;
};
#endif
