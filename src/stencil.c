#include "stencil.h"
#include "rlgl.h"
#include "glfw3.h"

// Stencil masks
void BeginStencil()
{
    rlDrawRenderBatchActive();
    glEnable(GL_STENCIL_TEST);
}

void BeginStencilMask()
{
    glStencilFunc(GL_ALWAYS, 1, 0xFF); // Set any stencil to 1
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    glStencilMask(0xFF); // Write to stencil buffer
    glDepthMask(GL_FALSE); // Don't write to depth buffer
    glClear(GL_STENCIL_BUFFER_BIT); // Clear stencil buffer (0 by default)
}

void EndStencilMask()
{
    rlDrawRenderBatchActive();
    glStencilFunc(GL_EQUAL, 1, 0xFF); // Pass test if stencil value is 1
    glStencilMask(0x00); // Don't write anything to stencil buffer
    glDepthMask(GL_TRUE); // Write to depth buffer
}

void EndStencil()
{
    rlDrawRenderBatchActive();
    glDisable(GL_STENCIL_TEST);
}