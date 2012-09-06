/**
 * @file ip/cxx/HOG.cc
 * @date Sun Apr 22 21:13:44 2012 +0200
 * @author Laurent El Shafey <Laurent.El-Shafey@idiap.ch>
 *
 * Copyright (C) 2011-2012 Idiap Research Institute, Martigny, Switzerland
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3 of the License.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "bob/ip/HOG.h"
#include "bob/ip/Exception.h"
#include "bob/core/array_assert.h"

void bob::ip::hogComputeHistogram(const blitz::Array<double,2>& mag, 
  const blitz::Array<double,2>& ori, blitz::Array<double,1>& hist, 
  const bool init_hist, const bool full_orientation)
{
  // Checks input arrays
  bob::core::array::assertSameShape(mag, ori);

  // Computes histogram
  bob::ip::hogComputeHistogram_(mag, ori, hist, init_hist, full_orientation);
}

void bob::ip::hogComputeHistogram_(const blitz::Array<double,2>& mag, 
  const blitz::Array<double,2>& ori, blitz::Array<double,1>& hist, 
  const bool init_hist, const bool full_orientation)
{
  static const double range_orientation = (full_orientation? 2*M_PI : M_PI);
  const int nb_bins = hist.extent(0);

  // Initializes output to zero if required
  if(init_hist) hist = 0.;

  for(int i=0; i<mag.extent(0); ++i)
    for(int j=0; j<mag.extent(1); ++j)
    {
      double energy = mag(i,j);
      double orientation = ori(i,j);
      // Makes the orientation belongs to [0,2 PI] if required 
      // (because of atan2 implementation) 
      if(orientation < 0.) orientation += 2*M_PI; 
      // Makes the orientation belongs to [0, PI] if required 
      if( !full_orientation && (orientation > M_PI) ) orientation -= M_PI;

      // Computes "real" value of the closest bin
      double bin = orientation / range_orientation * nb_bins;
      // Computes the value of the "inferior" bin 
      // ("superior" bin corresponds to the one after the inferior bin)
      int bin_index = floor(bin);
      // Computes the weight for the "inferior" bin
      double weight = 1.-(bin-bin_index);

      // Updates the histogram (bilinearly)
      hist(bin_index % nb_bins) += weight * energy;
      hist((bin_index+1) % nb_bins) += (1. - weight) * energy;
    }
}

