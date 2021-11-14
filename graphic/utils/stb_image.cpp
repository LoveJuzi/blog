#define STB_IMAGE_IMPLEMENTATION
#include "utils/stb_image.h"

static struct InitStb {
    InitStb() {
        stbi_set_flip_vertically_on_load(true);
    }
} initStb;
