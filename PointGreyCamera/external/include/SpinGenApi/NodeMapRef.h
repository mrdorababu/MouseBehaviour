//=============================================================================
// Copyright © 2016 Point Grey Research, Inc. All Rights Reserved.
//
// This software is the confidential and proprietary information of 
// Point Grey Research, Inc. ("Confidential Information"). You shall not
// disclose such Confidential Information and shall use it only in 
// accordance with the terms of the "License Agreement" that you 
// entered into with PGR in connection with this software.
//
// UNLESS OTHERWISE SET OUT IN THE LICENSE AGREEMENT, THIS SOFTWARE IS 
// PROVIDED ON AN “AS-IS” BASIS AND POINT GREY RESEARCH INC. MAKES NO 
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

/*!
\file
\brief    Definition of CNodeMapRef
\ingroup Spinnaker_GenApi_PublicInterface
*/

#ifndef SPINNAKER_GENAPI_NODEMAPREF_H
#define SPINNAKER_GENAPI_NODEMAPREF_H

#include "GCString.h"
#include "Pointer.h"
#include "INodeMap.h"
#include "IDestroy.h"
#include "NodeMapFactory.h"

#include <cstdlib>

namespace Spinnaker
{
	namespace GenApi
	{
#   ifdef _WIN32

		// see below in the Linux branch
		inline IDestroy *CastToIDestroy(INodeMap *pNodeMap)
		{
			return dynamic_cast<IDestroy *>(pNodeMap);
		}

#   else
		//! makes sure the dynamic_cast operator is implemented in the DLL (due to a Linux bug)
		SPINNAKER_API IDestroy *CastToIDestroy(INodeMap *pNodeMap);
#   endif

		/**
		\brief Smartpointer template for NodeMaps with create function
		\ingroup GenApi_PublicInterface
		\tparam TCameraParams  The camera specific parameter class (auto generated from camera xml file)
		*/
		template<class TCameraParams>
		class SPINNAKER_API CNodeMapRefT : public TCameraParams
		{
		public:
			//! Constructor
			CNodeMapRefT(const GenICam::gcstring &DeviceName = "Device");

			//! Constructor
			CNodeMapRefT(INodeMap* pNodeMap, const GenICam::gcstring &DeviceName = "Device");

			//! Copy constructor
			CNodeMapRefT(const CNodeMapRefT& Them);

			//! Assignment
			CNodeMapRefT& operator=(const CNodeMapRefT& Them);

			//! Assignment of an INodeMap*
			CNodeMapRefT& operator=(INodeMap* pNodeMap);

			//! Destructor
			virtual ~CNodeMapRefT();

			//! Destroys the node map
			void _Destroy();

			//! Creates the object from a XML file with given file name
			void _LoadXMLFromFile(const GenICam::gcstring &FileName);

			//! Creates the object from a ZIP'd XML file with given file name
			void _LoadXMLFromZIPFile(const GenICam::gcstring &ZipFileName);

			//! Creates the object from a ZIP'd XML file given in a string
			void _LoadXMLFromZIPData(const void* zipData, size_t zipSize);

			//! Creates the object from a XML target and an inject file with given file name
			void _LoadXMLFromFileInject(const GenICam::gcstring &TargetFileName, const GenICam::gcstring &InjectFileName);

			//! Creates the object from XML data given in a string
			void _LoadXMLFromString(const GenICam::gcstring& XMLData);

			//! Creates the object from XML data given in a string with injection
			void _LoadXMLFromStringInject(const GenICam::gcstring& TargetXMLDataconst, const GenICam::gcstring& InjectXMLData);

			//! Gets a list of supported schema versions
			/*! Each list entry is a string with the format "{Major}.{Minor}" were {Major} and {Minor} are integers
			Example: {"1.1", "1.2"} indicates that the schema v1.1 and v1.2 are supported.
			The SubMinor version number is not given since it is for fully compatible bug fixes only
			*/
			virtual void _GetSupportedSchemaVersions(GenICam::gcstring_vector &SchemaVersions);

			//! Get device name
			virtual GenICam::gcstring _GetDeviceName();

			//! Fires nodes which have a polling time
			virtual void _Poll(int64_t ElapsedTime);

			//! Clears the cache of the camera description files
			static bool _ClearXMLCache();

			//----------------------------------------------------------------
			// INodeMap
			//----------------------------------------------------------------

			//! Retrieves all nodes in the node map
			virtual void _GetNodes(NodeList_t &Nodes);

			//! Retrieves the node from the central map by name
			virtual INode* _GetNode(const GenICam::gcstring& key);

			//! Invalidates all nodes
			virtual void _InvalidateNodes();

			//! Connects a port to a port node with given name
			virtual bool _Connect(IPort* pPort, const GenICam::gcstring& PortName);

			//! Connects a port to the standard port "Device"
			virtual bool _Connect(IPort* pPort);

			//! Pointer to the NodeMap
			INodeMap *_Ptr;

		private:
			//! The name of this device
			GenICam::gcstring _DeviceName;

			//ATTENTION: not thread safe
			int* _pRefCount;
			void Release();
			void Attach(INodeMap *pNodeMap, const GenICam::gcstring &DeviceName, int* pRefCount);
		};



		template<class TCameraParams>
		inline CNodeMapRefT<TCameraParams>::CNodeMapRefT(const GenICam::gcstring &DeviceName)
			: _Ptr(NULL)
			, _DeviceName(DeviceName)
			, _pRefCount(NULL)
		{
		}

		template<class TCameraParams>
		inline CNodeMapRefT<TCameraParams>::CNodeMapRefT(INodeMap *pNodeMap, const GenICam::gcstring &DeviceName)
			: _Ptr(NULL)
			, _DeviceName(DeviceName)
			, _pRefCount(NULL)
		{
			assert(pNodeMap);
			Attach(pNodeMap, DeviceName, pNodeMap ? new int(0) : NULL);
		}

		template<class TCameraParams>
		GenApi::CNodeMapRefT<TCameraParams>::CNodeMapRefT(const CNodeMapRefT& Them)
			: _Ptr(NULL)
			, _DeviceName()
			, _pRefCount(NULL)
		{
			Attach(Them._Ptr, Them._DeviceName, Them._pRefCount);
		}

		//ATTENTION: not thread safe
		template<class TCameraParams>
		void GenApi::CNodeMapRefT<TCameraParams>::Attach(INodeMap *pNodeMap, const GenICam::gcstring &DeviceName, int* pRefCount)
		{
			// Must be empty
			assert(_Ptr == NULL);
			assert(_pRefCount == NULL);

			//always copy device name
			if (&_DeviceName != &DeviceName) //if not assigning member itself
			{
				_DeviceName = DeviceName;
			}

			// Attach
			if (pNodeMap)
			{
				assert(pRefCount);
				if (pRefCount)
				{
					++*pRefCount;

					//assign new node map data
					_Ptr = pNodeMap;
					_pRefCount = pRefCount;

					// Initialize the references
					TCameraParams::_Initialize(_Ptr);
				}
			}
		}

		//ATTENTION: not thread safe
		template<class TCameraParams>
		void GenApi::CNodeMapRefT<TCameraParams>::Release()
		{
			if (_Ptr)
			{
				// Copy node map data for eventual later destruction
				INodeMap* pToDel = _Ptr;
				int* pRefCount = _pRefCount;

				// Clear
				_pRefCount = NULL;
				_Ptr = NULL;
				_DeviceName = "Device";

				assert(pRefCount);
				// Check if destruction is required
				if (pRefCount)
				{
					assert(*pRefCount > 0);
					--*pRefCount;
					if (*pRefCount == 0)
					{
						// We do not need this anymore, all references are gone
						delete pRefCount;
						pRefCount = NULL;

						// Destroy the node map finally
						GenApi::IDestroy *pDestroy = CastToIDestroy(pToDel);
						assert(pDestroy);
						pDestroy->Destroy(); //must not throw
					}
				}
			}
			else
			{
				// Must not have a refcount when no node map is there.
				assert(_pRefCount == NULL);
			}
		}

		template<class TCameraParams>
		CNodeMapRefT<TCameraParams>& GenApi::CNodeMapRefT<TCameraParams>::operator=(INodeMap* pNodeMap)
		{
			Release();
			assert(pNodeMap);
			if (pNodeMap)
			{
				Attach(pNodeMap, pNodeMap->GetDeviceName(), new int(0));
			}

			return *this;
		}

		template<class TCameraParams>
		CNodeMapRefT<TCameraParams>& GenApi::CNodeMapRefT<TCameraParams>::operator=(const CNodeMapRefT& Them)
		{
			Release();
			Attach(Them._Ptr, Them._DeviceName, Them._pRefCount);
			return *this;
		}

		template<class TCameraParams>
		inline CNodeMapRefT<TCameraParams>::~CNodeMapRefT()
		{
			Release();
		}

		template<class TCameraParams>
		inline void CNodeMapRefT<TCameraParams>::_Destroy()
		{
			Release();
		}

		template<class TCameraParams>
		inline void _LoadXMLFromFile(const GenICam::gcstring &FileName);

		template<class TCameraParams>
		inline void _LoadXMLFromZIPFile(const GenICam::gcstring &ZipFileName);

		template<class TCameraParams>
		inline void _LoadXMLFromFileInject(const GenICam::gcstring &TargetFileName, const GenICam::gcstring &InjectFileName);

		template<class TCameraParams>
		inline void _LoadXMLFromString(const GenICam::gcstring& XMLData);


		template<class TCameraParams>
		inline void _LoadXMLFromZIPData(const void* zipData, size_t zipSize);

		template<class TCameraParams>
		inline void _LoadXMLFromStringInject(const GenICam::gcstring& TargetXMLData, const GenICam::gcstring& InjectXMLData);

		template<class TCameraParams>
		inline void _GetSupportedSchemaVersions(GenICam::gcstring_vector &SchemaVersions);

		template<class TCameraParams>
		inline GenICam::gcstring _GetDeviceName();

		template<class TCameraParams>
		inline void _Poll(int64_t ElapsedTime);

		template<class TCameraParams>
		inline void _GetNodes(NodeList_t &Nodes);

		template<class TCameraParams>
		inline INode* _GetNode(const GenICam::gcstring& key);

		template<class TCameraParams>
		inline void _InvalidateNodes();

		template<class TCameraParams>
		inline bool _Connect(IPort* pPort, const GenICam::gcstring& PortName);

		template<class TCameraParams>
		inline bool _Connect(IPort* pPort);

		template<class TCameraParams>
		inline bool _ClearXMLCache();

		/**
		\brief Empty base class used by class CNodeMapRef as generic template argument
		\ingroup GenApi_PublicInterface
		*/
		class CGeneric_XMLLoaderParams
		{
		protected:
			virtual void _Initialize(GenApi::INodeMap*) {}
		};


		/**
		\brief Smartpointer for NodeMaps with create function
		\ingroup GenApi_PublicInterface
		\note This class is a simple typedef definition. The class syntax is only used,
		because Doxygen has to generate a useful documentation.
		*/
		class CNodeMapRef : public CNodeMapRefT<GenApi::CGeneric_XMLLoaderParams>
		{
		public:
			//! Constructor
			CNodeMapRef(const GenICam::gcstring &DeviceName = "Device")
				: CNodeMapRefT<GenApi::CGeneric_XMLLoaderParams>(DeviceName)
			{
			}

			//! Constructor
			CNodeMapRef(INodeMap* pNodeMap, const GenICam::gcstring &DeviceName = "Device")
				: CNodeMapRefT<GenApi::CGeneric_XMLLoaderParams>(pNodeMap, DeviceName)
			{
			}

			//! Copy constructor
			CNodeMapRef(const CNodeMapRef& Them)
				: CNodeMapRefT<GenApi::CGeneric_XMLLoaderParams>(Them)
			{
			}

			//! Assignment
			CNodeMapRef& operator=(const CNodeMapRef& Them)
			{
				CNodeMapRefT<GenApi::CGeneric_XMLLoaderParams>::operator=(Them);
				return *this;
			}

			//! Assignment of an INodeMap*
			CNodeMapRef& operator=(INodeMap* pNodeMap)
			{
				CNodeMapRefT<GenApi::CGeneric_XMLLoaderParams>::operator=(pNodeMap);
				return *this;
			}
		};
	}
}

#endif // ifndef SPINNAKER_GENAPI_NODEMAPPTR_H
