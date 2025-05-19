#include "updatemodelmaterialcommand.h"
#include "mainwindow.h"

UpdateModelMaterialCommand::UpdateModelMaterialCommand(MainWindow* window, Sahara::Material* material, Parameter parameter, const QColor& value)
    : _window(window)
    , _material(material)
    , _parameter(parameter)
    , _colorAfter(value)
{
    setText("update material");

    switch (parameter) {
        case EmissionColor:
            _colorBefore = _material->emission();
            break;
        case AmbientColor:
            _colorBefore = _material->ambient();
            break;
        case DiffuseColor:
            _colorBefore = _material->diffuse();
            break;
        case SpecularColor:
            _colorBefore = _material->specular();
            break;
        case Shininess:
        case Image:
            break;
        }
}

UpdateModelMaterialCommand::UpdateModelMaterialCommand(MainWindow* window, Sahara::Material* material, float shininess)
    : _window(window)
    , _material(material)
    , _parameter(Parameter::Shininess)
    , _valueBefore(material->shininess())
    , _valueAfter(shininess)
{
    setText("update material");
}

UpdateModelMaterialCommand::UpdateModelMaterialCommand(MainWindow* window, Sahara::Material* material, const QString& imageURI)
    : _window(window)
    , _material(material)
    , _parameter(Parameter::Image)
    , _uriAfter(imageURI)
{
    Sahara::OpenGLMaterial* glMaterial;

    if ((glMaterial = dynamic_cast<Sahara::OpenGLMaterial*>(material))) {
        _uriBefore = (*glMaterial->image())->uri();
    }
#ifdef VULKAN
    else {
        Sahara::VulkanMaterial* vkMaterial;
        vkMaterial = dynamic_cast<Sahara::VulkanMaterial*>(material);
        _uriBefore = (*vkMaterial->image())->uri();
    }
#endif
    setText("update material");
}

void UpdateModelMaterialCommand::undo()
{
    Sahara::OpenGLMaterial* glMaterial;
    switch (_parameter) {
        case EmissionColor:
            _material->setEmission(_colorBefore);
            break;
        case AmbientColor:
            _material->setAmbient(_colorBefore);
            break;
        case DiffuseColor:
            _material->setDiffuse(_colorBefore);
            break;
        case SpecularColor:
            _material->setSpecular(_colorBefore);
            break;
        case Shininess:
            _material->setShininess(_valueBefore);
            break;
        case Image:
            if ((glMaterial = dynamic_cast<Sahara::OpenGLMaterial*>(_material))) {
                Sahara::OpenGLImage* oldImage = dynamic_cast<Sahara::OpenGLImage*>(*glMaterial->image());
                Sahara::OpenGLImage* newImage = new Sahara::OpenGLImage(oldImage->id(), _uriBefore);
                glMaterial->image() = newImage;
                delete oldImage;
            }
#ifdef VULKAN
            else {
                Sahara::VulkanMaterial* vkMaterial;
                vkMaterial = dynamic_cast<Sahara::VulkanMaterial*>(_material);
                Sahara::VulkanImage* oldImage = dynamic_cast<Sahara::VulkanImage*>(*vkMaterial->image());
                Sahara::VulkanImage* newImage = new Sahara::VulkanImage(dynamic_cast<Sahara::VulkanRenderer*>(_window->renderer()), oldImage->id(), _uriBefore);
                vkMaterial->image() = newImage;
                delete oldImage;
            }
#endif
            break;
    }

    _window->updateModelFields();
}

void UpdateModelMaterialCommand::redo()
{
    Sahara::OpenGLMaterial* glMaterial;
    switch (_parameter) {
        case EmissionColor:
            _material->setEmission(_colorAfter);
            break;
        case AmbientColor:
            _material->setAmbient(_colorAfter);
            break;
        case DiffuseColor:
            _material->setDiffuse(_colorAfter);
            break;
        case SpecularColor:
            _material->setSpecular(_colorAfter);
            break;
        case Shininess:
            _material->setShininess(_valueAfter);
            break;
        case Image:
            if ((glMaterial = dynamic_cast<Sahara::OpenGLMaterial*>(_material))) {
                Sahara::OpenGLImage* oldImage = dynamic_cast<Sahara::OpenGLImage*>(*glMaterial->image());
                Sahara::OpenGLImage* newImage = new Sahara::OpenGLImage(oldImage->id(), _uriAfter);
                glMaterial->image() = newImage;
                delete oldImage;
            }
#ifdef VULKAN
            else {
                Sahara::VulkanMaterial* vkMaterial;
                vkMaterial = dynamic_cast<Sahara::VulkanMaterial*>(_material);
                Sahara::VulkanImage* oldImage = dynamic_cast<Sahara::VulkanImage*>(*vkMaterial->image());
                Sahara::VulkanImage* newImage = new Sahara::VulkanImage(dynamic_cast<Sahara::VulkanRenderer*>(_window->renderer()), oldImage->id(), _uriAfter);
                vkMaterial->image() = newImage;
                delete oldImage;
            }
#endif
            break;
    }

    _window->updateModelFields();
}

int UpdateModelMaterialCommand::id() const
{
    return 'M';
}

bool UpdateModelMaterialCommand::mergeWith(const QUndoCommand* other)
{
    if (_parameter != Parameter::Shininess) {
        return false;
    }

    const UpdateModelMaterialCommand* ummc;
    if ((ummc = dynamic_cast<const UpdateModelMaterialCommand*>(other))) {
        if (ummc->_material == _material && ummc->_parameter == _parameter) {
            _valueAfter = ummc->_valueAfter;
            return true;
        }
    }

    return false;
}
