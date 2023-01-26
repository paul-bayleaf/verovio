/////////////////////////////////////////////////////////////////////////////
// Name:        resetfunctor.cpp
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "resetfunctor.h"

//----------------------------------------------------------------------------

#include "arpeg.h"
#include "custos.h"
#include "dot.h"
#include "editorial.h"
#include "elementpart.h"
#include "ending.h"
#include "f.h"
#include "ftrem.h"
#include "hairpin.h"
#include "layer.h"
#include "ligature.h"
#include "mrest.h"
#include "rest.h"
#include "section.h"
#include "slur.h"
#include "staff.h"
#include "stem.h"
#include "syl.h"
#include "tempo.h"
#include "tuplet.h"
#include "turn.h"
#include "verse.h"

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// ResetDataFunctor
//----------------------------------------------------------------------------

ResetDataFunctor::ResetDataFunctor() {}

FunctorCode ResetDataFunctor::VisitAccid(Accid *accid)
{
    // Call parent one too
    this->VisitLayerElement(accid);
    accid->PositionInterface::InterfaceResetData(*this, accid);

    return FUNCTOR_CONTINUE;
}

FunctorCode ResetDataFunctor::VisitArpeg(Arpeg *arpeg)
{
    // Call parent one too
    this->VisitControlElement(arpeg);

    PlistInterface *interface = arpeg->GetPlistInterface();
    assert(interface);
    return interface->InterfaceResetData(*this, arpeg);
}

FunctorCode ResetDataFunctor::VisitArtic(Artic *artic)
{
    // Call parent one too
    this->VisitLayerElement(artic);

    artic->SetDrawingPlace(STAFFREL_NONE);

    return FUNCTOR_CONTINUE;
}

FunctorCode ResetDataFunctor::VisitBeam(Beam *beam)
{
    // Call parent one too
    this->VisitLayerElement(beam);
    beam->BeamDrawingInterface::Reset();

    beam->m_beamSegment.Reset();
    beam->SetStemSameasBeam(NULL);

    // We want the list of the ObjectListInterface to be regenerated
    beam->Modify();

    return FUNCTOR_CONTINUE;
}

FunctorCode ResetDataFunctor::VisitChord(Chord *chord)
{
    // Call parent one too
    this->VisitLayerElement(chord);

    // We want the list of the ObjectListInterface to be regenerated
    chord->Modify();
    return FUNCTOR_CONTINUE;
}

FunctorCode ResetDataFunctor::VisitControlElement(ControlElement *controlElement)
{
    // Call parent one too
    this->VisitFloatingObject(controlElement);

    // Pass it to the pseudo functor of the interface
    if (controlElement->HasInterface(INTERFACE_ALT_SYM)) {
        AltSymInterface *interface = controlElement->GetAltSymInterface();
        assert(interface);
        return interface->InterfaceResetData(*this, controlElement);
    }
    if (controlElement->HasInterface(INTERFACE_LINKING)) {
        LinkingInterface *interface = controlElement->GetLinkingInterface();
        assert(interface);
        return interface->InterfaceResetData(*this, controlElement);
    }

    return FUNCTOR_CONTINUE;
}

FunctorCode ResetDataFunctor::VisitCustos(Custos *custos)
{
    // Call parent one too
    this->VisitLayerElement(custos);
    custos->PositionInterface::InterfaceResetData(*this, custos);

    return FUNCTOR_CONTINUE;
}

FunctorCode ResetDataFunctor::VisitDot(Dot *dot)
{
    // Call parent one too
    this->VisitLayerElement(dot);
    dot->PositionInterface::InterfaceResetData(*this, dot);

    dot->m_drawingPreviousElement = NULL;
    dot->m_drawingNextElement = NULL;

    return FUNCTOR_CONTINUE;
}

FunctorCode ResetDataFunctor::VisitDots(Dots *dots)
{
    // Call parent one too
    this->VisitLayerElement(dots);

    dots->ResetMapOfDotLocs();
    dots->IsAdjusted(false);

    return FUNCTOR_CONTINUE;
}

FunctorCode ResetDataFunctor::VisitEditorialElement(EditorialElement *editorialElement)
{
    if (editorialElement->IsSystemMilestone()) {
        editorialElement->SystemMilestoneInterface::InterfaceResetData(*this);
    }

    return FUNCTOR_CONTINUE;
}

FunctorCode ResetDataFunctor::VisitEnding(Ending *ending)
{
    this->VisitFloatingObject(ending);

    ending->SystemMilestoneInterface::InterfaceResetData(*this);

    return FUNCTOR_CONTINUE;
}

FunctorCode ResetDataFunctor::VisitF(F *f)
{
    this->VisitTextElement(f);

    TimeSpanningInterface *interface = f->GetTimeSpanningInterface();
    assert(interface);
    return interface->InterfaceResetData(*this, f);
}

FunctorCode ResetDataFunctor::VisitFlag(Flag *flag)
{
    this->VisitLayerElement(flag);

    flag->m_drawingNbFlags = 0;

    return FUNCTOR_CONTINUE;
}

FunctorCode ResetDataFunctor::VisitFloatingObject(FloatingObject *floatingObject)
{
    floatingObject->ResetDrawing();
    floatingObject->SetDrawingGrpId(0);

    // Pass it to the pseudo functor of the interface
    if (floatingObject->HasInterface(INTERFACE_TIME_SPANNING)) {
        TimeSpanningInterface *interface = floatingObject->GetTimeSpanningInterface();
        assert(interface);
        return interface->InterfaceResetData(*this, floatingObject);
    }
    else if (floatingObject->HasInterface(INTERFACE_TIME_POINT)) {
        TimePointInterface *interface = floatingObject->GetTimePointInterface();
        assert(interface);
        return interface->InterfaceResetData(*this, floatingObject);
    }
    return FUNCTOR_CONTINUE;
}

FunctorCode ResetDataFunctor::VisitFTrem(FTrem *fTrem)
{
    // Call parent one too
    this->VisitLayerElement(fTrem);

    fTrem->m_beamSegment.Reset();

    // We want the list of the ObjectListInterface to be regenerated
    fTrem->Modify();
    return FUNCTOR_CONTINUE;
}

FunctorCode ResetDataFunctor::VisitHairpin(Hairpin *hairpin)
{
    // Call parent one too
    this->VisitControlElement(hairpin);

    hairpin->SetLeftLink(NULL);
    hairpin->SetRightLink(NULL);
    hairpin->SetDrawingLength(0);

    return FUNCTOR_CONTINUE;
}

FunctorCode ResetDataFunctor::VisitLayer(Layer *layer)
{
    layer->SetCrossStaffFromAbove(false);
    layer->SetCrossStaffFromBelow(false);
    return FUNCTOR_CONTINUE;
}

FunctorCode ResetDataFunctor::VisitLayerElement(LayerElement *layerElement)
{
    layerElement->SetIsInBeamSpan(false);
    layerElement->SetDrawingCueSize(false);
    layerElement->m_crossStaff = NULL;
    layerElement->m_crossLayer = NULL;

    // Pass it to the pseudo functor of the interface
    LinkingInterface *interface = layerElement->GetLinkingInterface();
    assert(interface);
    interface->InterfaceResetData(*this, layerElement);

    return FUNCTOR_CONTINUE;
}

FunctorCode ResetDataFunctor::VisitLigature(Ligature *ligature)
{
    // Call parent one too
    this->VisitLayerElement(ligature);

    ligature->m_drawingShapes.clear();

    // We want the list of the ObjectListInterface to be regenerated
    ligature->Modify();
    return FUNCTOR_CONTINUE;
}

FunctorCode ResetDataFunctor::VisitMeasure(Measure *measure)
{
    measure->m_timestampAligner.Reset();
    measure->SetDrawingEnding(NULL);
    return FUNCTOR_CONTINUE;
}

FunctorCode ResetDataFunctor::VisitMRest(MRest *mRest)
{
    // Call parent one too
    this->VisitLayerElement(mRest);
    mRest->PositionInterface::InterfaceResetData(*this, mRest);

    return FUNCTOR_CONTINUE;
}

FunctorCode ResetDataFunctor::VisitNote(Note *note)
{
    // Call parent one too
    this->VisitLayerElement(note);
    note->PositionInterface::InterfaceResetData(*this, note);

    note->SetDrawingLoc(0);
    note->SetFlippedNotehead(false);
    note->SetStemSameasNote(NULL);
    note->SetStemSameasRole(SAMEAS_NONE);

    return FUNCTOR_CONTINUE;
}

FunctorCode ResetDataFunctor::VisitRest(Rest *rest)
{
    // Call parent one too
    this->VisitLayerElement(rest);
    rest->PositionInterface::InterfaceResetData(*this, rest);

    return FUNCTOR_CONTINUE;
}

FunctorCode ResetDataFunctor::VisitSection(Section *section)
{
    this->VisitFloatingObject(section);

    if (section->IsSystemMilestone()) {
        section->SystemMilestoneInterface::InterfaceResetData(*this);
    }

    return FUNCTOR_CONTINUE;
}

FunctorCode ResetDataFunctor::VisitSlur(Slur *slur)
{
    // Call parent one too
    this->VisitControlElement(slur);

    slur->SetDrawingCurveDir(SlurCurveDirection::None);

    return FUNCTOR_CONTINUE;
}

FunctorCode ResetDataFunctor::VisitStaff(Staff *staff)
{
    staff->m_timeSpanningElements.clear();
    staff->ClearLedgerLines();
    return FUNCTOR_CONTINUE;
}

FunctorCode ResetDataFunctor::VisitStem(Stem *stem)
{
    // Call parent one too
    this->VisitLayerElement(stem);

    stem->SetDrawingStemDir(STEMDIRECTION_NONE);
    stem->SetDrawingStemLen(0);

    return FUNCTOR_CONTINUE;
}

FunctorCode ResetDataFunctor::VisitSyl(Syl *syl)
{
    // Call parent one too
    this->VisitLayerElement(syl);

    syl->m_nextWordSyl = NULL;

    // Pass it to the pseudo functor of the interface
    return syl->TimeSpanningInterface::InterfaceResetData(*this, syl);
}

FunctorCode ResetDataFunctor::VisitSystemMilestone(SystemMilestoneEnd *systemMilestoneEnd)
{
    this->VisitFloatingObject(systemMilestoneEnd);

    systemMilestoneEnd->SetMeasure(NULL);

    return FUNCTOR_CONTINUE;
}

FunctorCode ResetDataFunctor::VisitTempo(Tempo *tempo)
{
    // Call parent one too
    this->VisitControlElement(tempo);

    tempo->ResetDrawingXRelative();

    return FUNCTOR_CONTINUE;
}

FunctorCode ResetDataFunctor::VisitTuplet(Tuplet *tuplet)
{
    // Call parent one too
    this->VisitLayerElement(tuplet);

    // We want the list of the ObjectListInterface to be regenerated
    tuplet->Modify();

    tuplet->SetDrawingLeft(NULL);
    tuplet->SetDrawingRight(NULL);

    return FUNCTOR_CONTINUE;
}

FunctorCode ResetDataFunctor::VisitTurn(Turn *turn)
{
    // Call parent one too
    this->VisitControlElement(turn);

    turn->m_drawingEndElement = NULL;

    return FUNCTOR_CONTINUE;
}

FunctorCode ResetDataFunctor::VisitVerse(Verse *verse)
{
    // Call parent one too
    this->VisitLayerElement(verse);

    verse->SetDrawingLabelAbbr(NULL);

    return FUNCTOR_CONTINUE;
}

//----------------------------------------------------------------------------
// ResetHorizontalAlignmentFunctor
//----------------------------------------------------------------------------

ResetHorizontalAlignmentFunctor::ResetHorizontalAlignmentFunctor() {}

FunctorCode ResetHorizontalAlignmentFunctor::VisitAccid(Accid *accid)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode ResetHorizontalAlignmentFunctor::VisitArpeg(Arpeg *arpeg)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode ResetHorizontalAlignmentFunctor::VisitBeam(Beam *beam)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode ResetHorizontalAlignmentFunctor::VisitBeamSpan(BeamSpan *beamSpan)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode ResetHorizontalAlignmentFunctor::VisitCustos(Custos *custos)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode ResetHorizontalAlignmentFunctor::VisitDot(Dot *dot)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode ResetHorizontalAlignmentFunctor::VisitDots(Dots *dots)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode ResetHorizontalAlignmentFunctor::VisitFloatingObject(FloatingObject *floatingObject)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode ResetHorizontalAlignmentFunctor::VisitLayer(Layer *layer)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode ResetHorizontalAlignmentFunctor::VisitLayerElement(LayerElement *layerElement)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode ResetHorizontalAlignmentFunctor::VisitMeasure(Measure *measure)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode ResetHorizontalAlignmentFunctor::VisitMRest(MRest *mRest)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode ResetHorizontalAlignmentFunctor::VisitNote(Note *note)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode ResetHorizontalAlignmentFunctor::VisitRest(Rest *rest)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode ResetHorizontalAlignmentFunctor::VisitScoreDef(ScoreDef *scoreDef)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode ResetHorizontalAlignmentFunctor::VisitSystem(System *system)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode ResetHorizontalAlignmentFunctor::VisitTuplet(Tuplet *tuplet)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode ResetHorizontalAlignmentFunctor::VisitTupletBracket(TupletBracket *tupletBracket)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode ResetHorizontalAlignmentFunctor::VisitTupletNum(TupletNum *tupletNum)
{
    return FUNCTOR_CONTINUE;
}

} // namespace vrv
