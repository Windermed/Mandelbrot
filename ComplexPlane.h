#include <SFML/Graphics.hpp>
#include <complex>
#include <sstream>

using namespace sf;
using namespace std;

// max number of iterations before a point is considered on set.
const unsigned int MAX_ITER = 64;

// defauilt width and height of complex plane window.
const float BASE_WIDTH = 4.0;
const float BASE_HEIGHT = 4.0;

// zoom that is applied per click.
const float BASE_ZOOM = 0.5;

// tracks whether the fractual needs to be recalculated or is ready to display.
enum class State
{
	CALCULATING,
	DISPLAYING
};

/*
struct Colors
{
	Uint8 r;
	Uint8 g;
	Uint8 b;
};
*/ // maybe not.

class ComplexPlane : public Drawable 
{
	
public:
	/* complex plane constructor */
	ComplexPlane(int pixelWidth, int pixelHeight);

	/* draws the vertex array to render window. */
	void draw(RenderTarget& target, RenderStates states) const override;

	/* recalculates pixel colors */
	void UpdateRender();

	/* to make thing easier. */
	void RenderSection(int startRow, int endRow);

	/* shrinks the view window to zoom IN. */
	void ZoomIn();

	/* increase hte view window to zoom out. */
	void ZoomOut();

	/* recenters the complex plane on the clicked pixel location. */
	void SetCenter(Vector2i mousePixel);

	/* stores current mouse position in a vector for HUD */
	void SetMouseLocation(Vector2i mousePixel);

	/* updates the text(like mouse coords, etc). */
	void LoadText(Text& text);

	/* Returns the number of iterations for a coordinate.*/
	size_t CountIterations(Vector2f coord);

	/* maps an iteration count to rgb color value using 5 colors */
	void IterationsToRGB(size_t count, Uint8& r, Uint8& g, Uint8& b);

	/* we'll map a pixel location to a coordinate */
	Vector2f MapPixelsToCoords(Vector2i mousePixel);

private:
	// screen wiuth in pixels
	int m_pixelWidth;
	// screen height in pixels
	int m_PixelHeight;

	// aspect ratio to prevent distortion.
	float m_aspectRatio;
	// one vertex per screen pixel.
	VertexArray m_vArray;

	// mouse location.
	Vector2f m_MouuseLocation;
	// current center of plane view.
	Vector2f m_plane_center;

	// current width and height of plane view
	Vector2f m_plane_size;

	// this will track current zoom level
	int m_zoomCount;

	// tracks wheteher or not the plane needs recalculating.
	State m_State;
};

