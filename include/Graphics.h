#ifndef AR_GRAPHICS_H_
#define AR_GRAPHICS_H_

#ifndef __APPLE__
#  include <GL/glew.h>
#endif
#include <GL/freeglut.h>

// camera
#include <cmath>
#include <eigen/Eigen/Dense>

// mesh
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cfloat>
#include <cmath>
#include <vector>
#include <eigen/Eigen/Dense>

using namespace std;

namespace opengl_handler{

  class Graphics
  {
    private:
      ;
    public:
      Graphics();
      ~Graphics();

  };

}

#endif // AR_GRAPHICS_H_
