#include "batchcreator.h"

#include "scene_p.h"

namespace GraphView {

void BatchCreator::apply() {
    _sceneData->undoStack->push(this);
}

} // namespace GraphView
