/*
 * Hydrogen
 * Copyright(c) 2002-2008 by Alex >Comix< Cominu [comix@users.sourceforge.net]
 *
 * http://www.hydrogen-music.org
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY, without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */
#ifndef H2CORE_SEQSCRIPT_H
#define H2CORE_SEQSCRIPT_H

#include "SeqEvent.h"
#include "SeqScriptIterator.h"

/**
 * The SeqScript is how the sequencer communicates events to sequence
 * clients.  It communicates a series of SeqEvents (typically note on/note
 * off stuff).  Each event is indexed with an offset to the first frame of the
 * current process() cycle.
 *
 * The SeqScript is primarily intended for triggering events in the
 * sampler.  However, it could easily be used to drive midi output, or multiple
 * sound generators, or loggers, etc.
 */

namespace H2Core
{
    class SeqScriptPrivate;

    /**
     * A FIFO list of events from the sequencer for sequencer clients.
     *
     * It would be apropos if this conformed to the STL Sequence concept... or
     * something like it.  (But, not necc. a random access sequence.)  The
     * intended use for this class is something like this:
     *
     *     sequencer_process(uint32_t nframes)
     *     {
     *         SeqScript seq;
     *         TransportPosition pos;
     *         SeqInputInterface* pSongSeq;  // i.e. Song -> SeqEvents
     *         SeqInputInterface* pMidiInput;
     *         SeqInputInterface* pGuiInput;
     *         SeqClientInterface* pSampler; // i.e. H2Core::Sampler
     *         SeqClientInterface* pMidiOut; // i.e. H2Core::MidiOutput
     *
     *         // Get events from input sources
     *
     *         pSongSeq->process(seq, pos, nframes);
     *         pMidiInput->process(seq, pos, nframes);
     *         pGuiInput->process(seq, pos, nframes);
     *
     *         // Send events to the clients to be processed.
     *
     *         pSampler->process(seq.begin_const(),
     *                           seq.end_const(nframes),
     *                           nframes);
     *         pMidiOut->process(seq.begin_const(),
     *                           seq.end_const(nframes),
     *                           nframes);
     *
     *         seq.consumed(nframes);
     *     }
     *
     *     SeqClientImplementation::process(SeqScriptConstIterator beg,
     *                                      SeqScriptConstIterator end,
     *                                      uint32_t nframes)
     *     {
     *         SeqScriptConstIterator k;
     *
     *         for( k=beg ; k != end ; ++k ) {
     *             // process events
     *         }
     *     }
     *
     * This way, SeqScript is protected from manipulation by SequencerClients.
     *
     */
    class SeqScript
    {
    public:
        typedef uint32_t size_type;
        typedef SeqEvent value_type;
        typedef SeqEvent::frame_type frame_type;
        typedef SeqScriptIterator iterator;
        typedef SeqScriptConstIterator const_iterator;
        SeqScript();
        ~SeqScript();

        /**
         * Properties
         */
        /// True if there are no events.  False otherwise.
        bool empty() const;
        /// Returns the number of events stored.
        size_type size() const;
        /// Returns the number of events before frame 'before_frame'
        size_type size(frame_type before_frame) const;
        /// Returns the number of SeqEvents reserved in a buffer.
        size_type max_size() const;

        /**
         * METHODS FOR THE SEQUENCER
         *
         * These methods should *only* be used by the sequencer.  Sequencer
         * clients should *not* use these.  This could possibly be enforced by
         * always passing the sequencer script to the client as
         * 'const SequencerScript& script'.
         */
        /// Reserves memory for a specific number of events.  This
        /// will almost always reallocate memory and should not be
        /// called from any realtime events.
        void reserve(size_type events);
        /// Insert an event (SeqScript handles sorting)
        void insert(const value_type& event);
        /// Remove an event.  It remains to be seen if this invalidates
        /// any iterators.
        void remove(const value_type& event);  // where matches event
        void remove(value_type* event);        // pointer to event
        /// Removes all the events before 'before_frame' (i.e. after
        /// they have been processed.
        void consumed(frame_type before_frame);
        /// Clears out all queued events.
        void clear();

        /// Schedules the note on/off pair, and cancel any note-off events
        /// currently between the two (unless this note is already scheduled to
        /// be interrupted by another note-on event).
        void insert_note(const value_type& event, frame_type length);

        /// Provides a read-only view of this sequence script from frames 0 to
        /// nframes.
        SeqScriptConstIterator begin_const() const;
        SeqScriptConstIterator end_const() const;
        SeqScriptConstIterator end_const(frame_type nframes) const;

        #warning "Temporary method SeqScript::at()"
        // This is a temporary method so things will compile...
        // should last until things get fully defined.
        value_type& at(frame_type frame);
        const value_type& at(frame_type frame) const;

    private:
        SeqScriptPrivate* d;
    };

}

#endif // H2CORE_SEQSCRIPT_H
