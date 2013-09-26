/* This file is part of the Zenipex Library (zenilib).
 * Copyright (C) 2011 Mitchell Keith Bloch (bazald).
 *
 * zenilib is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * zenilib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with zenilib.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <zeni_audio.h>

#include <algorithm>
#include <iostream>

#include <Zeni/Define.h>

#if defined(_DEBUG) && defined(_WINDOWS)
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

#include <Zeni/Singleton.hxx>

namespace Zeni {

  template class Singleton<Sound_Source_Pool>;

  Sound_Source_Pool * Sound_Source_Pool::create() {
    return new Sound_Source_Pool;
  }

  Singleton<Sound_Source_Pool>::Uninit Sound_Source_Pool::g_uninit;
  Singleton<Sound_Source_Pool>::Reinit Sound_Source_Pool::g_reinit;

  Sound_Source_Pool::Sound_Source_Pool()
    : m_replacement_policy(new Replacement_Policy()),
    delete_m_replacement_policy(true),
    m_muted(false)
  {
    Sound::remove_post_reinit(&g_reinit);

    // Ensure Sound is initialized
    Sound &sr = get_Sound();

    sr.lend_pre_uninit(&g_uninit);
    sr.lend_post_reinit(&g_reinit);
  }

  Sound_Source_Pool::~Sound_Source_Pool() {
    Sound::remove_pre_uninit(&g_uninit);

    purge();

    for(std::vector<Sound_Source *>::iterator it = m_handles.begin();
        it != m_handles.end();
        ++it)
    {
      (*it)->m_remove_from_Pool_on_destruction = false;
    }

    if(delete_m_replacement_policy)
      delete m_replacement_policy;
  }

  bool Sound_Source_Pool::Replacement_Policy::operator()(const Sound_Source &lhs, const Sound_Source &rhs) const {
    if(&lhs == &rhs)
      return false;

    if(!rhs.is_playing())
      return false;
    if(!lhs.is_playing())
      return true;

    if(rhs.get_priority() < lhs.get_priority())
      return false;
    if(lhs.get_priority() < rhs.get_priority())
      return true;

    if(rhs.get_gain() < lhs.get_gain())
      return false;
    if(lhs.get_gain() < rhs.get_gain())
      return true;

    return lhs.get_unstop_time() < rhs.get_unstop_time();
  }

  bool Sound_Source_Pool::Replacement_Policy::operator()(const Sound_Source * const &lhs, const Sound_Source * const &rhs) const {
    return (*this)(*lhs, *rhs);
  }
  
  Sound_Source_Pool::Positional_Replacement_Policy::Positional_Replacement_Policy(const Point3f &listener_position_)
    : listener_position(listener_position_)
  {
  }

  bool Sound_Source_Pool::Positional_Replacement_Policy::operator()(const Sound_Source &lhs, const Sound_Source &rhs) const {
    if(&lhs == &rhs)
      return false;

    if(!rhs.is_playing())
      return false;
    if(!lhs.is_playing())
      return true;

    if(rhs.get_priority() < lhs.get_priority())
      return false;
    if(lhs.get_priority() < rhs.get_priority())
      return true;

    if(rhs.get_priority() < lhs.get_priority())
      return false;
    if(lhs.get_priority() < rhs.get_priority())
      return true;

    const float lhs_gain = lhs.calculate_gain(listener_position);
    const float rhs_gain = rhs.calculate_gain(listener_position);

    if(rhs_gain < lhs_gain)
      return false;
    if(lhs_gain < rhs_gain)
      return true;

    return lhs.get_unstop_time() < rhs.get_unstop_time();
  }

  const Sound_Source_Pool::Replacement_Policy & Sound_Source_Pool::get_Replacement_Policy() const {
    return *m_replacement_policy;
  }
  
  void Sound_Source_Pool::give_Replacement_Policy(Sound_Source_Pool::Replacement_Policy * const &replacement_policy) {
    set_Replacement_Policy(replacement_policy);
    delete_m_replacement_policy = true;
  }
  
  void Sound_Source_Pool::lend_Replacement_Policy(Sound_Source_Pool::Replacement_Policy * const &replacement_policy) {
    set_Replacement_Policy(replacement_policy);
    delete_m_replacement_policy = false;
  }

  void Sound_Source_Pool::pause_all() {
    for(std::vector<Sound_Source *>::iterator it = m_handles.begin();
        it != m_handles.end();
        ++it)
    {
      if((*it)->is_playing())
        (*it)->pause();
    }
  }

  void Sound_Source_Pool::unpause_all() {
    for(std::vector<Sound_Source *>::iterator it = m_handles.begin();
        it != m_handles.end();
        ++it)
    {
      if((*it)->is_paused())
        (*it)->play();
    }
  }

  void Sound_Source_Pool::purge() {
    for(std::vector<Sound_Source *>::iterator it = m_playing_and_destroying.begin();
        it != m_playing_and_destroying.end();
        ++it)
    {
      delete *it;
    }
    m_playing_and_destroying.clear();

    destroy_all_hw();
  }

  static void sound_quicksort(
    const std::vector<Sound_Source *>::iterator &begin,
    const std::vector<Sound_Source *>::iterator &end,
    const Sound_Source_Pool::Replacement_Policy &policy)
  {
    typedef std::vector<Sound_Source *>::difference_type diff_t;
    const diff_t size = end - begin;

    if(size < 2)
      return;

    std::vector<Sound_Source *> less, greater;
    std::vector<Sound_Source *>::iterator pivot = begin + size / 2;
    Sound_Source * const pivotValue = *pivot;

    for(std::vector<Sound_Source *>::iterator x = begin; x != end; ++x)
      if(x != pivot) {
        if(policy(*x, pivotValue))
          less.push_back(*x);
        else
          greater.push_back(*x);
      }

    const std::vector<Sound_Source *>::iterator less_end = std::copy(less.begin(), less.end(), begin);
    *less_end = pivotValue;
    const std::vector<Sound_Source *>::iterator greater_begin = less_end + 1;
    std::copy(greater.begin(), greater.end(), greater_begin);

    less.clear();
    greater.clear();

    sound_quicksort(begin, less_end, policy);
    sound_quicksort(greater_begin, end, policy);
  }

  void Sound_Source_Pool::update() {
    /*** Handle the playing and destroying ***/

    std::vector<Sound_Source *> keepers;
    keepers.reserve(m_playing_and_destroying.size());
    for(std::vector<Sound_Source *>::iterator it = m_playing_and_destroying.begin();
        it != m_playing_and_destroying.end();
        ++it)
    {
      if((*it)->is_stopped()) {
        ZENI_LOGD("Removing Sound_Source from Sound_Source_Pool");
        delete *it;
      }
      else
        keepers.push_back(*it);
    }
    m_playing_and_destroying.swap(keepers);

    /*** If muted, skip the rest of the update ***/

    if(m_muted)
      return;

    /*** Do the regular update ***/

    std::vector<Sound_Source_HW *> unassigned_hw;
    const size_t needed_hw = m_handles.size();
    size_t given_hw = 0;

    /*** Find out how much Sound_Source_HW is needed ***/

    for(std::vector<Sound_Source *>::iterator it = m_handles.begin();
        it != m_handles.end();
        ++it)
    {
      if((*it)->is_assigned())
        ++given_hw;
    }

    /*** Acquire more Sound_Source_HW if needed (and if possible) ***/

//     ZENI_LOGD("Sound_Source_Pool has " + ulltoa(given_hw) + "/" + ulltoa(needed_hw));
    while(needed_hw > given_hw
#ifdef TEST_NASTY_CONDITIONS
      && given_hw < NASTY_SOUND_SOURCE_CAP
#endif
      )
    {
      Sound_Source_HW * const sshw = Sound_Source_HW::Try_Construct();

      if(sshw) {
        ZENI_LOGD("Sound_Source_HW added to Sound_Source_Pool");
        unassigned_hw.push_back(sshw);
        ++given_hw;
      }
      else {
        ZENI_LOGD("Failed to add Sound_Source_HW to Sound_Source_Pool");
        break;
      }
    }

    /*** Strip Sound_Source_HW from low priority 'Sound_Source's, as needed ***/

    const size_t cut = needed_hw - given_hw;

    if(cut) {
      sound_quicksort(m_handles.begin(), m_handles.end(), *m_replacement_policy);

      for(size_t i = 0; i != cut; ++i) {
        Sound_Source &source = *m_handles[i];

        if(source.is_assigned())
          unassigned_hw.push_back(source.unassign());
      }
    }

    /*** Assign Sound_Source_HW to high priority 'Sound_Source's ***/

    std::vector<Sound_Source_HW *>::iterator jt = unassigned_hw.begin();

    for(size_t i = cut; i != needed_hw; ++i) {
      Sound_Source &source = *m_handles[i];

      if(!source.is_assigned()) {
        source.assign(**jt);
        ++jt;
      }
    }
  }

  void Sound_Source_Pool::play_and_destroy(Sound_Source * const &sound_source) {
    m_playing_and_destroying.push_back(sound_source);
    sound_source->play();
  }

  void Sound_Source_Pool::set_Replacement_Policy(Sound_Source_Pool::Replacement_Policy * const &replacement_policy) {
    assert(replacement_policy);

    if(delete_m_replacement_policy)
      delete m_replacement_policy;

    m_replacement_policy = replacement_policy;
  }

  void Sound_Source_Pool::insert_Sound_Source(Sound_Source &sound_source) {
    m_handles.push_back(&sound_source);
  }

  void Sound_Source_Pool::remove_Sound_Source(Sound_Source &sound_source) {
    sound_source.stop();

    if(sound_source.m_hw) {
      delete sound_source.m_hw;
      sound_source.m_hw = 0;
    }

    m_handles.erase(std::find(m_handles.begin(), m_handles.end(), &sound_source));
  }

  void Sound_Source_Pool::destroy_all_hw() {
    for(std::vector<Sound_Source *>::iterator it = m_handles.begin();
        it != m_handles.end();
        ++it)
    {
      if((*it)->is_assigned()) {
        Sound_Source_HW * hw = (*it)->unassign();
        hw->stop();
        delete hw;
      }
    }
  }

  Sound_Source_Pool & get_Sound_Source_Pool() {
    return Sound_Source_Pool::get();
  }

  void play_sound(
    const String &sound_name,
    const float &pitch,
    const float &gain,
    const Point3f &position,
    const Vector3f &velocity)
  {
    get_Sound_Source_Pool().play_and_destroy(new Sound_Source(get_Sounds()[sound_name], pitch, gain, position, velocity));
  }

}

#include <Zeni/Undefine.h>
