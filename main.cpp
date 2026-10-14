#include "threepp/threepp.hpp"
using namespace threepp;
int main() {
    Canvas canvas("threepp");
    GLRenderer renderer(canvas);
    auto scene = Scene::create();
    scene->background = Color::white;
    auto camera = PerspectiveCamera::create(60, canvas.aspect(), 0.1f, 1000);
    camera->position.z = 5;
    canvas.onWindowResize([&](WindowSize size) {
    camera->aspect = size.aspect();
    camera->updateProjectionMatrix();
    renderer.setSize(size);
    });
    auto light = DirectionalLight::create();
    light->position.set(1, 2, 3);
    scene->add(light);
    scene->add(AmbientLight::create(0xffffff, 0.4f)); // hvitt lys med 40 % styrke
    auto geometry = BoxGeometry::create(1, 1, 1);
    auto material = MeshStandardMaterial::create();
    material->color = Color::orange;
    auto box = Mesh::create(geometry, material);
    scene->add(box);
    Clock clock;
    canvas.animate([&] {
    const float dt = clock.getDelta();
    box->rotation.y += 1.0f * dt; // én radian per sekund
    renderer.render(*scene, *camera);
    });
}