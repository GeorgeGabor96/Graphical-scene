#include "ParticleMaster.hpp"

namespace parti
{
    ParticleMaster::ParticleMaster() {};

    ParticleMaster::ParticleMaster(ParticleTexture texture, gps::Shader shader, glm::mat4 projectionMatrix, float pps, float speed, float gravityComplient, float lifeLength, float scale)
    {
        this->texture = texture;
        renderer = parti::ParticleRenderer(shader, projectionMatrix);
        this->pps = pps;
        this->averageSpeed = speed;
        this->gravityComplient = gravityComplient;
        this->averageLifeLength = lifeLength;
        this->averageScale = scale;
        randomRotation = false;
        speedError = 0;
        lifeError = 0;
        scaleError = 0;
        directionDeviation = 0;
        direction = glm::vec3(0, 0, 0);
    }   

    void ParticleMaster::update(glm::vec3 camPos, float frameTime)
    {
        std::vector<Particle*>::iterator iterator;
        
        for (iterator = particles.begin(); iterator != particles.end(); )
        {
            if (false == (*iterator)->update(camPos, frameTime))
            {
                iterator = particles.erase(iterator);
            }
            else
            {
                iterator++;
            }
        }

        insertionSort();
    }

    void ParticleMaster::insertionSort()
    {
        int lenght = particles.size();
        int j;
        Particle * key = NULL;
        for (int i = 0; i < lenght; i++)
        {
             key = particles[i];
             j = i - 1;
             while (j >= 0 && particles[i]->getDistanceCamera() > key->getDistanceCamera())
             {
                 particles[j + 1] = particles[j];
                 j--;
             }
             particles[j + 1] = key;

        }
    }

    void ParticleMaster::setDirection(glm::vec3 direction, float deviation)
    {
        this->direction = glm::vec3(direction);
        this->directionDeviation = (float)(deviation * M_PI);
    }

    void ParticleMaster::randomizeRotation()
    {
        randomRotation = true;
    }

    void ParticleMaster::setSpeedError(float error)
    {
        this->speedError = error * averageSpeed;
    }

    void ParticleMaster::setLifeError(float error)
    {
        this->lifeError = error * averageLifeLength;
    }

    void ParticleMaster::setScaleError(float error)
    {
        this->scaleError = error * averageScale;
    }

    void ParticleMaster::renderParticles(glm::mat4 viewMatrix)
    {

        renderer.draw(texture, particles, viewMatrix);
    }

    void ParticleMaster::generateParticles(float frameTime, glm::vec3 systemCenter)
    {
        float delta = frameTime;
        float particlesToCreate = pps * delta;

        int count = (int)glm::floor(particlesToCreate);
        if (particlesToCreate > 1.0f)
        {
            particlesToCreate = 0;
        }

        float partialParticle = particlesToCreate;

        for (int i = 0; i < count; i++) {
           /* glm::vec3 velocity;
            if (direction.x == 0 && direction.y == 0 && direction.z == 0) {
                velocity = generateRandomUnitVectorWithinCone(direction, directionDeviation);
            }
            else {










                velocity = generateRandomUnitVectorWithinCone(direction, directionDeviation);
                velocity = generateRandomUnitVector();
            }
            velocity = glm::normalize(velocity);
            velocity *= (generateValue(averageSpeed, speedError));
*/
            float dirX = (float)((double)rand() / (RAND_MAX)) * 2.0f - 1.0f;
            //float dirY = velocity.y;
            float dirZ = (float)((double)rand() / (RAND_MAX)) * 2.0f - 1.0f;
            glm::vec3 velocity1 = glm::vec3(dirX, 1, dirZ);
            velocity1 = glm::normalize(velocity1);
            velocity1 *= (averageSpeed);
            float scale = generateValue(averageScale, scaleError);
            float lifeLength = generateValue(averageLifeLength, lifeError);
            Particle * particle = new Particle(texture, glm::vec3(systemCenter), glm::vec3(velocity1), gravityComplient, lifeLength, generateRotation(), scale);
            particles.push_back(particle);
        }
    }


    float ParticleMaster::generateValue(float average, float errorMargin)
    {
        float offset = ((float)((double)rand() / (RAND_MAX)) - 0.5f) * 2.0f * errorMargin;
        return average + offset;
    }

    float ParticleMaster::generateRotation()
    {
        if (randomRotation) {
            return (float)((double)rand() / (RAND_MAX)) * 360.0f;
        }
        else {
            return 0;
        }
    }

    glm::vec3 ParticleMaster::generateRandomUnitVectorWithinCone(glm::vec3 coneDirection, float angle)
    {
        float cosAngle = (float)cos(angle);
        float theta = (float)((float)((double)rand() / (RAND_MAX)) * 2.0f * M_PI);
        float z = cosAngle + ((float)((double)rand() / (RAND_MAX)) * (1 - cosAngle));
        float rootOneMinusZSquared = (float)sqrt(1 - z * z);
        float x = (float)(rootOneMinusZSquared * cos(theta));
        float y = (float)(rootOneMinusZSquared * sin(theta));

        glm::vec4 direction = glm::vec4(x, y, z, 1);
        if (coneDirection.x != 0 || coneDirection.y != 0 || (coneDirection.z != 1 && coneDirection.z != -1)) {
            glm::vec3 rotateAxis = glm::cross(coneDirection, glm::vec3(0, 0, 1));
            rotateAxis = glm::normalize(rotateAxis);
            float rotateAngle = (float)acos(glm::dot(coneDirection, glm::vec3(0, 0, 1)));
            glm::mat4 rotationMatrix;
            rotationMatrix = glm::rotate(rotationMatrix, -rotateAngle, rotateAxis);
            direction = rotationMatrix * direction;
        }
        else if (coneDirection.z == -1) {
            direction.z *= -1;
        }
        return glm::vec3(direction);
    }

    glm::vec3 ParticleMaster::generateRandomUnitVector()
    {
        float theta = (float)((float)((double)rand() / (RAND_MAX)) * 2.0f * M_PI);
        float z = (float)((double)rand() / (RAND_MAX) * 2.0f) - 1.0f;
        float rootOneMinusZSquared = (float)sqrt(1 - z * z);
        float x = (float)(rootOneMinusZSquared * cos(theta));
        float y = (float)(rootOneMinusZSquared * sin(theta));
        return glm::vec3(x, y, z);
    }

    void ParticleMaster::count()
    {
        printf("%d\n", particles.size());
    }

}