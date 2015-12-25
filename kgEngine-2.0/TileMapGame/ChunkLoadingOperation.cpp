#include "ChunkLoadingOperation.h"
using namespace std;
using namespace sf;
using namespace tgui;

namespace kg
{
	void ChunkLoadingOperation::execute_main()
	{
		m_saveInformation = saveManager.loadEntitySaveInformationFromFile( chunkToLoad.getSavename() );

		if( !m_saveInformation )
		{
			m_entities = chunkGenerator.generateChunk( engine, world, chunkToLoad.getPosition() );
		}
	}
}