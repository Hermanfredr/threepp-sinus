#include "threepp/threepp.hpp"
#include <vector>
#include <memory>

using namespace threepp;

int main() {
    Canvas canvas("threepp - 8 kuber");
    GLRenderer renderer(canvas);

    auto scene = Scene::create();
    scene->background = Color::aliceblue;

    auto camera = PerspectiveCamera::create(60, canvas.aspect(), 0.1f, 1000);
    camera->position.z = 8;

    canvas.onWindowResize([&](WindowSize size) {
        camera->aspect = size.aspect();
        camera->updateProjectionMatrix();
        renderer.setSize(size);
    });

    auto light = DirectionalLight::create();
    light->position.set(1, 2, 3);
    scene->add(light);

    scene->add(AmbientLight::create(0xffffff, 0.4f));

    // En liste som holder på alle kubene.
    std::vector<std::shared_ptr<Mesh>> kuber;

    auto geometry = BoxGeometry::create(1, 1, 1);
    auto material = MeshStandardMaterial::create();
    material->color = Color::blue;

    for (int rad = 0; rad < 2; rad++) {
        for (int kolonne = 0; kolonne < 4; kolonne++) {
            auto box = Mesh::create(geometry, material);

            box->position.x = kolonne * 2.0f - 3.0f;
            box->position.y = 1.0f - rad * 2.0f;

            scene->add(box);
            kuber.push_back(box);
        }
    }

    Clock clock;

    canvas.animate([&] {
        const float dt = clock.getDelta();

        for (auto& box : kuber) {
            box->rotation.y += 1.0f * dt;
        }

        renderer.render(*scene, *camera);
    });
}
    