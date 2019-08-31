
#include <QEditor/Panels/MeshEntityPanel.h>

env::Vec3 CMeshEntityPanel::m_editEmissiveColor(0.0f, 0.0f, 0.0f);
env::Vec3 CMeshEntityPanel::m_editDiffuseColor(0.0f, 0.0f, 0.0f);
env::Vec3 CMeshEntityPanel::m_editEnvColor(0.0f, 0.0f, 0.0f);

float CMeshEntityPanel::m_editEmissiveEnergy = 0.0f;
float CMeshEntityPanel::m_editDiffuseEnergy = 0.0f;
float CMeshEntityPanel::m_editEnvReflectivity = 0.0f;