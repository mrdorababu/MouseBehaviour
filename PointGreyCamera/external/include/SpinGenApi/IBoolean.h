//=============================================================================
// Copyright � 2016 Point Grey Research, Inc. All Rights Reserved.
//
// This software is the confidential and proprietary information of 
// Point Grey Research, Inc. ("Confidential Information"). You shall not
// disclose such Confidential Information and shall use it only in 
// accordance with the terms of the "License Agreement" that you 
// entered into with PGR in connection with this software.
//
// UNLESS OTHERWISE SET OUT IN THE LICENSE AGREEMENT, THIS SOFTWARE IS 
// PROVIDED ON AN �AS-IS� BASIS AND POINT GREY RESEARCH INC. MAKES NO 
// REPRESENTATIONS OR WARRANTIES ABOUT THE SOFTWARE, EITHER EXPRESS 
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO ANY IMPLIED WARRANTIES OR 
// CONDITIONS OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, OR 
// NON-INFRINGEMENT. POINT GREY RESEARCH INC. SHALL NOT BE LIABLE FOR ANY 
// DAMAGES, INCLUDING BUT NOT LIMITED TO ANY DIRECT, INDIRECT, INCIDENTAL, 
// SPECIAL, PUNITIVE, OR CONSEQUENTIAL DAMAGES, OR ANY LOSS OF PROFITS, 
// REVENUE, DATA OR DATA USE, ARISING OUT OF OR IN CONNECTION WITH THIS 
// SOFTWARE OR OTHERWISE SUFFERED BY YOU AS A RESULT OF USING, MODIFYING 
// OR DISTRIBUTING THIS SOFTWARE OR ITS DERIVATIVES.
//=============================================================================

#ifndef SPINNAKER_GENAPI_IBOOLEAN_H
#define SPINNAKER_GENAPI_IBOOLEAN_H

#include "SpinnakerPlatform.h"
#include "Types.h"
#include "IValue.h"

#pragma warning ( push )
#pragma warning ( disable : 4251 ) // XXX needs to have dll-interface to be used by clients of class YYY

namespace Spinnaker
{
	namespace GenApi
	{
		//*************************************************************
		// IBoolean interface
		//*************************************************************

		/**
		\brief Interface for Boolean  properties
		\ingroup Spinnaker_GenApi_PublicInterface
		*/
		interface SPINNAKER_API_ABSTRACT IBoolean : virtual public IValue
		{
		public:
			//! Set node value
			/*!
			\param Value The value to set
			\param Verify Enables AccessMode and Range verification (default = true)
			*/
			virtual void SetValue(bool Value, bool Verify = true) = 0;

			//! Set node value
			virtual void operator=(bool Value){ SetValue(Value); }

			//! Get node value
			/*!
			\param Verify Enables Range verification (default = false). The AccessMode is always checked
			\param IgnoreCache If true the value is read ignoring any caches (default = false)
			\return The value read
			*/
			virtual bool GetValue(bool Verify = false, bool IgnoreCache = false) const = 0;

			//! Get node value
			virtual bool operator()() const { return GetValue(); }
		};
	}
}

#pragma warning ( pop )

#endif // ifndef SPINNAKER_GENAPI_IBOOLEAN_H
