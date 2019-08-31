/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */


#pragma once

#include <QEditor/UIGen/ParticleEmitterEntityPanel_ui.h>
#include <QEditor/UIGen/BoxParticleEmitterEntityPanel_ui.h>
#include <QEditor/Panels/PanelBase.h>

#include <QtWidgets/QFileDialog>
#include <QtWidgets/QColorDialog>

#undef UNICODE
#include <Engine/Entity/ParticleEmitterEntity.h>
#include <Engine/Entity/ParticleSystemEntity.h>
#define UNICODE 




class CParticleEmitterEntityPanel : public CPanel<Ui::ParticleEmitterEntityPanel>
{
    Q_OBJECT

public:
    CParticleEmitterEntityPanel(QWidget* pForm)
        : CPanel<Ui::ParticleEmitterEntityPanel>(pForm)
        , m_pEntity(nullptr)
    {
        connect(m_ui.pSpinBoxMaterialSequence, SIGNAL(valueChanged(int)), this, SLOT(slotSettingsChanged()));
        connect(m_ui.pSpinBoxSizeBegin, SIGNAL(valueChanged(int)), this, SLOT(slotSettingsChanged()));
        connect(m_ui.pSpinBoxSizeEnd, SIGNAL(valueChanged(int)), this, SLOT(slotSettingsChanged()));
        connect(m_ui.pSpinBoxFadeIn, SIGNAL(valueChanged(double)), this, SLOT(slotSettingsChanged()));
        connect(m_ui.pSpinBoxFadeOut, SIGNAL(valueChanged(double)), this, SLOT(slotSettingsChanged()));
        connect(m_ui.pSpinBoxAlphaBegin, SIGNAL(valueChanged(double)), this, SLOT(slotSettingsChanged()));
        connect(m_ui.pSpinBoxAlphaEnd, SIGNAL(valueChanged(double)), this, SLOT(slotSettingsChanged()));
        connect(m_ui.pSpinBoxBlendMode, SIGNAL(valueChanged(double)), this, SLOT(slotSettingsChanged()));
        connect(m_ui.pSpinBoxSizeRange, SIGNAL(valueChanged(double)), this, SLOT(slotSettingsChanged()));
        connect(m_ui.pSpinBoxParticleCount, SIGNAL(valueChanged(int)), this, SLOT(slotSettingsChanged()));
        connect(m_ui.pSpinBoxVelocityAngle, SIGNAL(valueChanged(double)), this, SLOT(slotSettingsChanged()));
        connect(m_ui.pSpinBoxVelocityMin, SIGNAL(valueChanged(double)), this, SLOT(slotSettingsChanged()));
        connect(m_ui.pSpinBoxVelocityMax, SIGNAL(valueChanged(double)), this, SLOT(slotSettingsChanged()));
        connect(m_ui.pSpinBoxLifetimeMin, SIGNAL(valueChanged(double)), this, SLOT(slotSettingsChanged()));
        connect(m_ui.pSpinBoxLifetimeMax, SIGNAL(valueChanged(double)), this, SLOT(slotSettingsChanged()));
        connect(m_ui.pSpinBoxEmitRate, SIGNAL(valueChanged(double)), this, SLOT(slotSettingsChanged()));
        connect(m_ui.pSpinBoxEmitDuration, SIGNAL(valueChanged(double)), this, SLOT(slotSettingsChanged()));
        connect(m_ui.pSpinBoxEmitDelay, SIGNAL(valueChanged(double)), this, SLOT(slotSettingsChanged()));
        connect(m_ui.pSpinBoxWarmupTime, SIGNAL(valueChanged(double)), this, SLOT(slotSettingsChanged()));
        connect(m_ui.pSpinBoxOrientationVelocityMin, SIGNAL(valueChanged(double)), this, SLOT(slotSettingsChanged()));
        connect(m_ui.pSpinBoxOrientationVelocityMax, SIGNAL(valueChanged(double)), this, SLOT(slotSettingsChanged()));
        connect(m_ui.pSpinBoxDistortionMin, SIGNAL(valueChanged(double)), this, SLOT(slotSettingsChanged()));
        connect(m_ui.pSpinBoxDistortionMax, SIGNAL(valueChanged(double)), this, SLOT(slotSettingsChanged()));
        connect(m_ui.pBtnColorBegin, SIGNAL(pressed()), this, SLOT(slotChooseBeginColor()));
        connect(m_ui.pBtnColorEnd, SIGNAL(pressed()), this, SLOT(slotChooseEndColor()));
        connect(m_ui.pRadioBtnFacingCamera, SIGNAL(clicked(bool)), this, SLOT(slotCheckBoxSettingsChanged()));
        connect(m_ui.pRadioBtnFacingEmitter, SIGNAL(clicked(bool)), this, SLOT(slotCheckBoxSettingsChanged()));
        connect(m_ui.pRadioBtnFacingHorizontal, SIGNAL(clicked(bool)), this, SLOT(slotCheckBoxSettingsChanged()));
        connect(m_ui.pRadioBtnFacingScreen, SIGNAL(clicked(bool)), this, SLOT(slotCheckBoxSettingsChanged()));
        connect(m_ui.pRadioBtnOrientationAlignToVelocity, SIGNAL(clicked(bool)), this, SLOT(slotCheckBoxSettingsChanged()));
        connect(m_ui.pRadioBtnOrientationRandom, SIGNAL(clicked(bool)), this, SLOT(slotCheckBoxSettingsChanged()));
        connect(m_ui.pBtnCaptureBoundingBox, SIGNAL(clicked(bool)), this, SLOT(slotCaptureBoundingBox()));
        connect(m_ui.pBtnPickParticleSystem, SIGNAL(clicked(bool)), this, SLOT(slotPickParticleSystem()));
        connect(m_ui.pCheckBoxGlobalSpaceParticles, SIGNAL(clicked(bool)), this, SLOT(slotCheckBoxSettingsChanged()));
        connect(m_ui.pCheckBoxEmitAllParticlesOnce, SIGNAL(clicked(bool)), this, SLOT(slotCheckBoxSettingsChanged()));
        connect(m_ui.pCheckBoxStartDisabled, SIGNAL(clicked(bool)), this, SLOT(slotCheckBoxSettingsChanged()));
        connect(m_ui.pGroupBoxStaticBoundingBox, SIGNAL(clicked(bool)), this, SLOT(slotCheckBoxSettingsChanged()));
    }

    //-----------------------------------------------------------------------------------
    virtual void syncEntityEvent(env::CEntity* pEntity)
    {
        if (!pEntity)
            return;

        m_pEntity = static_cast<env::CParticleEmitterEntity*>(pEntity);

        env::Vec4 colorBegin = (m_pEntity->GetColorBegin());
        env::Vec4 colorEnd = (m_pEntity->GetColorEnd());

        SetBtnColor(m_ui.pBtnColorBegin, env::DoGamma(env::Vec3(colorBegin)));
        SetBtnColor(m_ui.pBtnColorEnd, env::DoGamma(env::Vec3(colorEnd)));

        m_ui.pSpinBoxMaterialSequence->setValue(m_pEntity->GetMaterialSequence());
        m_ui.pSpinBoxSizeBegin->setValue(m_pEntity->GetParticleSizeBegin());
        m_ui.pSpinBoxSizeEnd->setValue(m_pEntity->GetParticleSizeEnd());
        m_ui.pSpinBoxFadeIn->setValue(m_pEntity->GetFadeInTime());
        m_ui.pSpinBoxFadeOut->setValue(m_pEntity->GetFadeOutTime());
        m_ui.pSpinBoxBlendMode->setValue(m_pEntity->GetBlendMode());
        m_ui.pSpinBoxSizeRange->setValue(m_pEntity->GetParticleSizeRange());
        m_ui.pSpinBoxParticleCount->setValue(m_pEntity->GetNumParticles());
        m_ui.pSpinBoxVelocityAngle->setValue(m_pEntity->GetVelocityAngle());
        m_ui.pSpinBoxVelocityMin->setValue(m_pEntity->GetVelocityBegin());
        m_ui.pSpinBoxVelocityMax->setValue(m_pEntity->GetVelocityEnd());
        m_ui.pSpinBoxDistortionMin->setValue(m_pEntity->GetDistortionMin());
        m_ui.pSpinBoxDistortionMax->setValue(m_pEntity->GetDistortionMax());
        m_ui.pSpinBoxLifetimeMin->setValue(m_pEntity->GetLifeTimeMin());
        m_ui.pSpinBoxLifetimeMax->setValue(m_pEntity->GetLifeTimeMax());
        m_ui.pSpinBoxEmitRate->setValue(m_pEntity->GetEmitRate());
        m_ui.pSpinBoxEmitDuration->setValue(m_pEntity->GetEmitDuration());
        m_ui.pSpinBoxEmitDelay->setValue(m_pEntity->GetEmitDelay());
        m_ui.pSpinBoxWarmupTime->setValue(m_pEntity->GetWarmUpTime());
        m_ui.pCheckBoxGlobalSpaceParticles->setChecked(m_pEntity->GetGlobalSpaceParticles());
        m_ui.pSpinBoxAlphaBegin->setValue(m_pEntity->GetColorBegin().w);
        m_ui.pSpinBoxAlphaEnd->setValue(m_pEntity->GetColorEnd().w);

        if (m_pEntity->GetOrientation() == env::PARTICLE_ORIENTATION_TO_VELOCITY)
            m_ui.pRadioBtnOrientationAlignToVelocity->setChecked(true);
        else
            m_ui.pRadioBtnOrientationRandom->setChecked(true);

        if (m_pEntity->GetFacing() == env::PARTICLE_FACING_CAMERA)
            m_ui.pRadioBtnFacingCamera->setChecked(true);
        else if (m_pEntity->GetFacing() == env::PARTICLE_FACING_EMITTER)
            m_ui.pRadioBtnFacingEmitter->setChecked(true);
        else if (m_pEntity->GetFacing() == env::PARTICLE_FACING_HORIZONTAL)
            m_ui.pRadioBtnFacingHorizontal->setChecked(true);
        else
            m_ui.pRadioBtnFacingScreen->setChecked(true);

        m_ui.pGroupBoxStaticBoundingBox->setChecked(!m_pEntity->GetDynamicBoundingBox());
        m_ui.pCheckBoxEmitAllParticlesOnce->setChecked(m_pEntity->GetEmitAllParticlesOnce());
        m_ui.pCheckBoxStartDisabled->setChecked(m_pEntity->GetStartDisabled());

        if (m_pEntity->GetParticleSystem())
        {
            m_ui.pGroupBoxColor->setEnabled(m_pEntity->GetParticleSystem()->GetRenderer() != "Distort");
            m_ui.pGroupBoxDistort->setEnabled(m_pEntity->GetParticleSystem()->GetRenderer() == "Distort");
        }
    }

    //-----------------------------------------------------------------------------------
    virtual QWidget* GetWidget() const { return m_ui.qEditorPanelBox; }

    public slots:

    //-----------------------------------------------------------------------------------
    virtual void slotSettingsChanged()
    {
        if (m_ui.pSpinBoxMaterialSequence->hasFocus())
            m_pEntity->SetMaterialSequence(m_ui.pSpinBoxMaterialSequence->value());

        if (m_ui.pSpinBoxSizeBegin->hasFocus())
            m_pEntity->SetParticleSizeBegin(m_ui.pSpinBoxSizeBegin->value());

        if (m_ui.pSpinBoxSizeEnd->hasFocus())
            m_pEntity->SetParticleSizeEnd(m_ui.pSpinBoxSizeEnd->value());

        if (m_ui.pSpinBoxFadeIn->hasFocus())
            m_pEntity->SetFadeInTime(m_ui.pSpinBoxFadeIn->value());

        if (m_ui.pSpinBoxFadeOut->hasFocus())
            m_pEntity->SetFadeOutTime(m_ui.pSpinBoxFadeOut->value());

        env::Vec4 colorBegin = m_pEntity->GetColorBegin();
        env::Vec4 colorEnd = m_pEntity->GetColorEnd();

        if (m_ui.pSpinBoxAlphaBegin->hasFocus())
            colorBegin.w = m_ui.pSpinBoxAlphaBegin->value();

        if (m_ui.pSpinBoxAlphaEnd->hasFocus())
            colorEnd.w = m_ui.pSpinBoxAlphaEnd->value();

        m_pEntity->SetColorBegin(colorBegin);
        m_pEntity->SetColorEnd(colorEnd);

        if (m_ui.pSpinBoxBlendMode->hasFocus())
            m_pEntity->SetBlendMode(m_ui.pSpinBoxBlendMode->value());

        if (m_ui.pSpinBoxSizeRange->hasFocus())
            m_pEntity->SetParticleSizeRange(m_ui.pSpinBoxSizeRange->value());

        if (m_ui.pSpinBoxParticleCount->hasFocus())
            m_pEntity->SetNumParticles(m_ui.pSpinBoxParticleCount->value());

        if (m_ui.pSpinBoxVelocityAngle->hasFocus())
            m_pEntity->SetVelocityAngle(m_ui.pSpinBoxVelocityAngle->value());

        if (m_ui.pSpinBoxVelocityMin->hasFocus())
            m_pEntity->SetVelocityBegin(m_ui.pSpinBoxVelocityMin->value());

        if (m_ui.pSpinBoxVelocityMax->hasFocus())
            m_pEntity->SetVelocityEnd(m_ui.pSpinBoxVelocityMax->value());

        if (m_ui.pSpinBoxLifetimeMin->hasFocus())
            m_pEntity->SetLifeTimeMin(m_ui.pSpinBoxLifetimeMin->value());

        if (m_ui.pSpinBoxLifetimeMax->hasFocus())
            m_pEntity->SetLifeTimeMax(m_ui.pSpinBoxLifetimeMax->value());

        if (m_ui.pSpinBoxEmitRate->hasFocus())
            m_pEntity->SetEmitRate(m_ui.pSpinBoxEmitRate->value());

        if (m_ui.pSpinBoxEmitDuration->hasFocus())
            m_pEntity->SetEmitDuration(m_ui.pSpinBoxEmitDuration->value());

        if (m_ui.pSpinBoxEmitDelay->hasFocus())
            m_pEntity->SetEmitDelay(m_ui.pSpinBoxEmitDelay->value());

        if (m_ui.pSpinBoxWarmupTime->hasFocus())
            m_pEntity->SetWarmUpTime(m_ui.pSpinBoxWarmupTime->value());

        if (m_ui.pSpinBoxOrientationVelocityMin->hasFocus())
            m_pEntity->SetOrientationSpeedMin(m_ui.pSpinBoxOrientationVelocityMin->value());

        if (m_ui.pSpinBoxOrientationVelocityMax->hasFocus())
            m_pEntity->SetOrientationSpeedMax(m_ui.pSpinBoxOrientationVelocityMax->value());

        if (m_ui.pSpinBoxDistortionMin->hasFocus())
            m_pEntity->SetDistortionMin(m_ui.pSpinBoxDistortionMin->value());

        if (m_ui.pSpinBoxDistortionMax->hasFocus())
            m_pEntity->SetDistortionMax(m_ui.pSpinBoxDistortionMax->value());
    }

    virtual void slotCheckBoxSettingsChanged()
    {
        if (!m_pEntity)
            return;

        m_pEntity->SetGlobalSpaceParticles(m_ui.pCheckBoxGlobalSpaceParticles->isChecked());
        m_pEntity->SetDynamicBoundingBox(!m_ui.pGroupBoxStaticBoundingBox->isChecked());
        m_pEntity->SetEmitAllParticlesOnce(m_ui.pCheckBoxEmitAllParticlesOnce->isChecked());
        m_pEntity->SetStartDisabled(m_ui.pCheckBoxStartDisabled->isChecked());

        if (m_ui.pRadioBtnOrientationAlignToVelocity->isChecked())
            m_pEntity->SetOrientation(env::PARTICLE_ORIENTATION_TO_VELOCITY);
        else if (m_ui.pRadioBtnOrientationRandom->isChecked())
            m_pEntity->SetOrientation(env::PARTICLE_ORIENTATION_RANDOM);

        if (m_ui.pRadioBtnFacingEmitter->isChecked())
            m_pEntity->SetFacing(env::PARTICLE_FACING_EMITTER);
        else if (m_ui.pRadioBtnFacingCamera->isChecked())
            m_pEntity->SetFacing(env::PARTICLE_FACING_CAMERA);
        else if (m_ui.pRadioBtnFacingHorizontal->isChecked())
            m_pEntity->SetFacing(env::PARTICLE_FACING_HORIZONTAL);
        else if (m_ui.pRadioBtnFacingScreen->isChecked())
            m_pEntity->SetFacing(env::PARTICLE_FACING_SCREEN);
    }

    //-----------------------------------------------------------------------------------
    virtual void slotPickParticleSystem()
    {

    }

    //-----------------------------------------------------------------------------------
    virtual void slotCaptureBoundingBox()
    {
        env::Vec3 vMin, vMax;
        m_pEntity->ComputeBoundingBox(vMin, vMax);

        // Map to local space
        m_pEntity->GetNode()->MapToLocal(vMin);
        m_pEntity->GetNode()->MapToLocal(vMax);

        m_pEntity->SetBoundingBoxMin(vMin);
        m_pEntity->SetBoundingBoxMax(vMax);
    }

    //-----------------------------------------------------------------------------------
    virtual void slotChooseBeginColor()
    {
        if (!m_pEntity)
            return;

        env::Vec3 diffuseColor = env::Vec3(m_pEntity->GetColorBegin());

        env::DoGamma(diffuseColor);

        gEditor->AcquireColorDialog(this, SLOT(slotBeginColorChanged(const QColor&)), diffuseColor, "Choose Begin Color");
    }

    //-----------------------------------------------------------------------------------
    virtual void slotBeginColorChanged(const QColor& color)
    {
        env::Vec3 diffuseColor = FromQColor(color);

        SetBtnColor(m_ui.pBtnColorBegin, diffuseColor);

        m_pEntity->SetColorBegin(env::Vec4(env::UndoGamma(diffuseColor), m_pEntity->GetColorBegin().w));
    }

    //-----------------------------------------------------------------------------------
    virtual void slotChooseEndColor()
    {
        if (!m_pEntity)
            return;

        env::Vec3 diffuseColor = env::Vec3(m_pEntity->GetColorEnd());

        env::DoGamma(diffuseColor);

        gEditor->AcquireColorDialog(this, SLOT(slotEndColorChanged(const QColor&)), diffuseColor, "Choose End Color");
    }

    //-----------------------------------------------------------------------------------
    virtual void slotEndColorChanged(const QColor& color)
    {
        env::Vec3 diffuseColor = FromQColor(color);

        SetBtnColor(m_ui.pBtnColorEnd, diffuseColor);

        m_pEntity->SetColorEnd(env::Vec4(env::UndoGamma(diffuseColor), m_pEntity->GetColorEnd().w));
    }


private:

    env::CParticleEmitterEntity* m_pEntity;

};




class CBoxParticleEmitterEntityPanel : public CPanel<Ui::BoxParticleEmitterEntityPanel>
{
    Q_OBJECT

public:
    CBoxParticleEmitterEntityPanel(QWidget* pForm)
        : CPanel<Ui::BoxParticleEmitterEntityPanel>(pForm)
        , m_pEntity(nullptr)
    {
        connect(m_ui.pSpinBoxEmitterMinX, SIGNAL(valueChanged(int)), this, SLOT(slotSettingsChanged()));
        connect(m_ui.pSpinBoxEmitterMinY, SIGNAL(valueChanged(int)), this, SLOT(slotSettingsChanged()));
        connect(m_ui.pSpinBoxEmitterMinZ, SIGNAL(valueChanged(int)), this, SLOT(slotSettingsChanged()));
        connect(m_ui.pSpinBoxEmitterMaxX, SIGNAL(valueChanged(int)), this, SLOT(slotSettingsChanged()));
        connect(m_ui.pSpinBoxEmitterMaxY, SIGNAL(valueChanged(int)), this, SLOT(slotSettingsChanged()));
        connect(m_ui.pSpinBoxEmitterMaxZ, SIGNAL(valueChanged(int)), this, SLOT(slotSettingsChanged()));

        connect(m_ui.pCheckBoxAlignScreenX, SIGNAL(clicked(bool)), this, SLOT(slotCheckboxChanged()));
        connect(m_ui.pCheckBoxAlignScreenY, SIGNAL(clicked(bool)), this, SLOT(slotCheckboxChanged()));
    }

    //-----------------------------------------------------------------------------------
    virtual void syncEntityEvent(env::CEntity* pEntity)
    {
        if (!pEntity)
            return;

        m_pEntity = static_cast<env::CBoxParticleEmitterEntity*>(pEntity);

        m_ui.pSpinBoxEmitterMinX->setValue(m_pEntity->GetEmitterBoxMin().x);
        m_ui.pSpinBoxEmitterMinY->setValue(m_pEntity->GetEmitterBoxMin().y);
        m_ui.pSpinBoxEmitterMinZ->setValue(m_pEntity->GetEmitterBoxMin().z);

        m_ui.pSpinBoxEmitterMaxX->setValue(m_pEntity->GetEmitterBoxMax().x);
        m_ui.pSpinBoxEmitterMaxY->setValue(m_pEntity->GetEmitterBoxMax().y);
        m_ui.pSpinBoxEmitterMaxZ->setValue(m_pEntity->GetEmitterBoxMax().z);

        m_ui.pCheckBoxAlignScreenX->setChecked(m_pEntity->GetAlignScreenX());
        m_ui.pCheckBoxAlignScreenY->setChecked(m_pEntity->GetAlignScreenY());
    }

public slots:

    virtual void slotCheckboxChanged()
    {
        m_pEntity->SetAlignScreenX(m_ui.pCheckBoxAlignScreenX->isChecked());
        m_pEntity->SetAlignScreenY(m_ui.pCheckBoxAlignScreenY->isChecked());
    }

    virtual void slotSettingsChanged()
    {
        env::Vec3 emitterMin = m_pEntity->GetEmitterBoxMin();
        env::Vec3 emitterMax = m_pEntity->GetEmitterBoxMax();

        if (m_ui.pSpinBoxEmitterMinX->hasFocus())
            emitterMin.x = static_cast<float>(m_ui.pSpinBoxEmitterMinX->value());

        if (m_ui.pSpinBoxEmitterMinY->hasFocus())
            emitterMin.y = static_cast<float>(m_ui.pSpinBoxEmitterMinY->value());

        if (m_ui.pSpinBoxEmitterMinZ->hasFocus())
            emitterMin.z = static_cast<float>(m_ui.pSpinBoxEmitterMinZ->value());

        if (m_ui.pSpinBoxEmitterMaxX->hasFocus())
            emitterMax.x = static_cast<float>(m_ui.pSpinBoxEmitterMaxX->value());

        if (m_ui.pSpinBoxEmitterMaxY->hasFocus())
            emitterMax.y = static_cast<float>(m_ui.pSpinBoxEmitterMaxY->value());

        if (m_ui.pSpinBoxEmitterMaxZ->hasFocus())
            emitterMax.z = static_cast<float>(m_ui.pSpinBoxEmitterMaxZ->value());

        m_pEntity->SetEmitterBoxMin(emitterMin);
        m_pEntity->SetEmitterBoxMax(emitterMax);
    }

private:

    env::CBoxParticleEmitterEntity* m_pEntity;

};