#include "renderer/raytracer/raytracer.h"
#include "renderer/renderer.h"
#include "resource.h"


namespace cg::renderer
{
	class ray_tracing_renderer : public renderer
	{
	public:
		virtual void init();
		virtual void destroy();

		virtual void update();
		virtual void render();

	protected:
		std::shared_ptr<cg::resource<cg::unsigned_color>> render_target;

		std::shared_ptr<cg::renderer::raytracer<cg::vertex, cg::unsigned_color>> raytracer;
		std::shared_ptr<cg::renderer::raytracer<cg::vertex, cg::unsigned_color>> shadow_raytracer;

		std::vector<cg::renderer::light> lights;
	};
}// namespace cg::renderer