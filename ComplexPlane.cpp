#include "ComplexPlane.h"
#include <thread>

ComplexPlane::ComplexPlane(int pixelWidth, int pixelHeight) 
{
    m_pixelWidth = pixelWidth;
    m_PixelHeight = pixelHeight;

    // aspect ratio to prevent the plane from being stretched
    m_aspectRatio = (float)pixelHeight / pixelWidth;

    // start at the origin point.
    m_plane_center = { 0, 0 };

    m_plane_size = { BASE_WIDTH, BASE_HEIGHT * m_aspectRatio };
    m_zoomCount = 0;
    m_State = State::CALCULATING;

    // setup the vertex array.
    m_vArray.setPrimitiveType(Points);
    m_vArray.resize(pixelWidth * pixelHeight);

}

void ComplexPlane::draw(RenderTarget& target, RenderStates states) const
{
    // draw the vertex array to our render target.
    target.draw(m_vArray);
}

// TODO: THREADING (this actually is killing my pc please send help)
void ComplexPlane::UpdateRender()
{
    if (m_State == State::CALCULATING)
    {
        // todo: look into lambdas

        unsigned int numThreads = thread::hardware_concurrency();
        vector<thread> theads;

        int rowsPerThread = m_PixelHeight / numThreads;

        // we'll spawn one thread per core. with each handling a range of rows.
        for (unsigned int t = 0; t < numThreads; t++)
        {
            int startRow = t * rowsPerThread;

            // the last thread takes any remaining rows.
            int endRow = (t == numThreads - 1) ? m_PixelHeight : startRow + rowsPerThread;
            theads.emplace_back(&ComplexPlane::RenderSection, this, startRow, endRow); // HERE WE GO
        }

        // we'll wait for all threads to finish before we continue.
        for (auto& t : theads)
        {
            t.join();
        }

        m_State = State::DISPLAYING;
    }
}

void ComplexPlane::RenderSection(int startRow, int endRow)
{
    for (int i = startRow; i < endRow; i++)
    {
        for (int j = 0; j < m_pixelWidth; j++)
        {
            // set up pixel position in the vertex array.
            m_vArray[j + i * m_pixelWidth].position = { (float)j, (float)i };

            // map pixel to complex plane.
            Vector2f coords = MapPixelsToCoords({ j, i });

            // then count how many iterations areb.
            size_t iters = CountIterations(coords);

            Uint8 r;
            Uint8 g;
            Uint8 b;

            IterationsToRGB(iters, r, g, b); // then convert iteration to rgb color.

            // apply color to pixel
            m_vArray[j + i * m_pixelWidth].color = { r, g, b };
        }
    }
}

// zoom in
void ComplexPlane::ZoomIn()
{
    m_zoomCount++;

    float xSize = BASE_WIDTH * pow(BASE_ZOOM, m_zoomCount);
    float ySize = BASE_HEIGHT * m_aspectRatio * pow(BASE_ZOOM, m_zoomCount);

    m_plane_size = { xSize, ySize };
    m_State = State::CALCULATING;
}

// zoom out. clamps at 0.
void ComplexPlane::ZoomOut()
{
    // fixed an annoying bug.
    if (m_zoomCount)
    {
        m_zoomCount--;
    }

    float xSize = BASE_WIDTH * pow(BASE_ZOOM, m_zoomCount);
    float ySize = BASE_HEIGHT * m_aspectRatio * pow(BASE_ZOOM, m_zoomCount);

    m_plane_size = { xSize, ySize };
    m_State = State::CALCULATING;
}

void ComplexPlane::SetCenter(Vector2i mousePixel)
{
    m_plane_center = MapPixelsToCoords(mousePixel);
    m_State = State::CALCULATING;
}

// stores current mouse position for HUD.
void ComplexPlane::SetMouseLocation(Vector2i mousePixel)
{
    m_MouuseLocation = MapPixelsToCoords(mousePixel);
}

void ComplexPlane::LoadText(Text& text)
{
    stringstream ss;

    ss << "Mandelbrot Set\n";
    ss << "Center: (" << m_plane_center.x << ", " << m_plane_center.y << ")\n";
    ss << "Cursor: (" << m_MouuseLocation.x << ", " << m_MouuseLocation.y << ")\n";  // almost forgot.
    ss << "Left click to Zoom in\n";
    ss << "Right click to zoom out!\n";

    text.setString(ss.str()); // then we set!
}


// todo: FIX!!
size_t ComplexPlane::CountIterations(Vector2f coord)
{
    complex<double> c(coord.x, coord.y);
    complex<double> z(0, 0);

    size_t i = 0;

    while (abs(z) < 2.0 && i < MAX_ITER)
    {
        z = z * z + c;
        i++; // whoops typo.
    }
    return i;
}

void ComplexPlane::IterationsToRGB(size_t count, Uint8& r, Uint8& g, Uint8& b)
{
    if (count == MAX_ITER)
    {
        r = g = b = 0;
        return;
    }

    // 5 color regions
    size_t region = MAX_ITER / 5;

    if (count < region)
    {
        r = 0;
        g = 0;
        b = 100 + count * 3;

    }
    else if (count < 2 * region)
    {
        r = 0;
        g = 150 + count;
        b = 150;
    }
    else if (count < 3 * region)
    {
        r = 0;
        g = 200;
        b = count * 2;
    }
    else if (count < 4 * region)
    {
        r = 200 + count;
        g = 200;
        b = 0;
    }
    else {
        r = 255;
        g = count * 2;
        b = 0;
    }
}

Vector2f ComplexPlane::MapPixelsToCoords(Vector2i mousePixel)
{
    float x = ((float)mousePixel.x / m_pixelWidth) * m_plane_size.x + (m_plane_center.x - m_plane_size.x / 2.0f);
    float y = ((float)(m_PixelHeight - mousePixel.y) / m_PixelHeight) * m_plane_size.y + (m_plane_center.y - m_plane_size.y / 2.0f);

    return { x, y };
}