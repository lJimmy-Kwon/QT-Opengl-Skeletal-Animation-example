#include "mesh.h"
Mesh::Mesh(QVector<Vertex> vertices, QVector<unsigned int> indices, QVector<Texture> textures, QVector<VertexBoneData> Bones)
{
    this->vertices = vertices;
    this->indices  = indices;
    this->textures = textures;

    for(int i = 0 ; i < vertices.size() ; i++){
        this->vertices[i].bones = Bones[i];
    }


    VAO = new QOpenGLVertexArrayObject;
    VBO = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    EBO = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);

    setupMesh();
}

Mesh::Mesh(const Mesh &mesh)
{

    vertices = mesh.vertices;
    indices  = mesh.indices;

    VAO = mesh.VAO;
    VBO = mesh.VBO;
    EBO = mesh.EBO;

    textures = mesh.textures;
}

Mesh::~Mesh()
{

}

void Mesh::Draw(QOpenGLShaderProgram &program)
{
    program.bind();
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;

    for( int i = 0; i < textures.size(); i++){
        glActiveTexture(GL_TEXTURE0 + i );

        QString number;
        QString name = textures[i].type;


        if(name == "texture_diffuse"){
            number = QString::number(diffuseNr++);
        }else if( name == "texture_specular"){
            number = QString::number(specularNr++);
        }
        program.setUniformValue((name + number).toStdString().c_str(), i);
        textures[i].image->bind();
    }


    program.setUniformValue("shininess", (int)textures[0].shininess);
    glActiveTexture(GL_TEXTURE0);

    VAO->bind();
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0 );
    VAO->release();

    program.release();

}

void Mesh::setupMesh()
{
    // Create Buffer ( Do not release util VAO is created)
    VBO->create();
    VBO->bind();
    VBO->setUsagePattern(QOpenGLBuffer::StaticDraw);
    VBO->allocate( vertices.constData(), vertices.size() * sizeof(Vertex));
    VBO->release();

    // create IndexBuffer
    EBO->create();
    EBO->bind();
    EBO->setUsagePattern(QOpenGLBuffer::StaticDraw);
    EBO->allocate( indices.constData(), indices.size() * sizeof(unsigned int));
    EBO->release();

    // Create Vertex Array Object
    VAO->create();
    VAO->bind();
    {
        VBO->bind();
        // set the vertex attribute pointers
        // vertex Positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

        // vertex normals
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

        // vertex texture coords
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

        // vertex bone Id location
        glEnableVertexAttribArray(3);
        glVertexAttribIPointer(3, 4, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, bones.IDs));

        // vertex bone Weights location
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bones.Weights));

        EBO->bind();
    }

    // Release ( unbind ) all
    VAO->release();
    EBO->release();
    VBO->release();

}
