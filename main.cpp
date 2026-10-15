#include "threepp/threepp.hpp"
#include <vector>
#include <memory>
#include <cmath>

using namespace threepp;

int main() {
    // ENDRE ANTALLET KUBER HER:
    const int antallKuber = 8;

    Canvas canvas("threepp - Sinus og fargeskifte");
    GLRenderer renderer(canvas);

    auto scene = Scene::create();
    scene->background = Color::aliceblue;

    // Kameraavstanden beregnes for å vise oppsettet med 8 kuber.
    const float pi = 3.14159265f;
    const float synsfelt = 60.0f;

    auto camera = PerspectiveCamera::create(
        synsfelt, canvas.aspect(), 0.1f, 1000
    );

    auto tilpassKamera = [&] {
        // Halv bredde og høyde med plass til rotasjon og bevegelse.
        const float halvBredde = 4.2f;
        const float halvHoyde = 3.0f;

        const float tangent = std::tan(synsfelt * pi / 360.0f);

        float avstandHoyde = halvHoyde / tangent;
        float avstandBredde = halvBredde / (tangent * camera->aspect);

        float avstand = avstandHoyde;

        if (avstandBredde > avstand) {
            avstand = avstandBredde;
        }

        camera->position.z = avstand + 1.0f;
    };

    tilpassKamera();

    canvas.onWindowResize([&](WindowSize size) {
        if (size.aspect() <= 0) {
            return;
        }

        camera->aspect = size.aspect();
        camera->updateProjectionMatrix();
        renderer.setSize(size);
        tilpassKamera();
    });

    auto light = DirectionalLight::create();
    light->position.set(1, 2, 3);
    scene->add(light);

    scene->add(AmbientLight::create(0xffffff, 0.4f));

    std::vector<std::shared_ptr<Mesh>> kuber;
    std::vector<float> startY;

    auto geometry = BoxGeometry::create(1, 1, 1);

    auto materialeRad1 = MeshStandardMaterial::create();
    materialeRad1->color = Color::blue;

    auto materialeRad2 = MeshStandardMaterial::create();
    materialeRad2->color = Color::blue;

    // Eksempel: 8 gir 4 + 4, mens 9 gir 5 + 4.
    int antallRad1 = (antallKuber + 1) / 2;
    int antallRad2 = antallKuber / 2;

    for (int rad = 0; rad < 2; rad++) {
        int antallIRaden = antallRad1;
        auto material = materialeRad1;

        if (rad == 1) {
            antallIRaden = antallRad2;
            material = materialeRad2;
        }

        for (int kolonne = 0; kolonne < antallIRaden; kolonne++) {
            auto box = Mesh::create(geometry, material);

            // Sentrer hver rad rundt x = 0.
            box->position.x =
                kolonne * 2.0f - (antallIRaden - 1) * 1.0f;

            box->position.y = 1.5f - rad * 3.0f;

            scene->add(box);
            kuber.push_back(box);
            startY.push_back(box->position.y);
        }
    }

    int fargeSteg = 0;

    canvas.onKeyPressed([&](KeyEvent event) {
        if (event.key == Key::SPACE) {
            fargeSteg++;

            if (fargeSteg == 1) {
                materialeRad1->color = Color::orange;
            }
            else if (fargeSteg == 2) {
                materialeRad2->color = Color::orange;
            }
            else {
                materialeRad1->color = Color::blue;
                materialeRad2->color = Color::blue;
                fargeSteg = 0;
            }
        }
    });

    float A = 0.5f;
    float f = 0.5f;

    Clock clock;
    float tid = 0;

    canvas.animate([&] {
        const float dt = clock.getDelta();
        tid += dt;

        for (int i = 0; i < antallKuber; i++) {
            float fase = 0;

            if (i % 2 == 1) {
                fase = pi;
            }

            kuber[i]->position.y =
                startY[i] + A * std::sin(2 * pi * f * tid + fase);

            kuber[i]->rotation.y += 1.0f * dt;
        }

        renderer.render(*scene, *camera);
    });
}