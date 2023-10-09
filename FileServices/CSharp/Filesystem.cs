// **********************************************************************
//
// Copyright (c) 2003-2008 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

// Ice version 3.3.0
// Generated from file `Filesystem.ice'

#if __MonoCS__

using _System = System;
using _Microsoft = Microsoft;
#else

using _System = global::System;
using _Microsoft = global::Microsoft;
#endif

namespace Filesystem
{
    public interface Node : Ice.Object, NodeOperations_, NodeOperationsNC_
    {
    }

    public class GenericError : Ice.UserException
    {
        #region Slice data members

        public string reason;

        #endregion

        #region Constructors

        public GenericError()
        {
        }

        public GenericError(_System.Exception ex__) : base(ex__)
        {
        }

        private void initDM__(string reason)
        {
            this.reason = reason;
        }

        public GenericError(string reason)
        {
            initDM__(reason);
        }

        public GenericError(string reason, _System.Exception ex__) : base(ex__)
        {
            initDM__(reason);
        }

        #endregion

        public override string ice_name()
        {
            return "Filesystem::GenericError";
        }

        #region Object members

        public override int GetHashCode()
        {
            int h__ = 0;
            if(reason != null)
            {
                h__ = 5 * h__ + reason.GetHashCode();
            }
            return h__;
        }

        public override bool Equals(object other__)
        {
            if(other__ == null)
            {
                return false;
            }
            if(object.ReferenceEquals(this, other__))
            {
                return true;
            }
            if(!(other__ is GenericError))
            {
                return false;
            }
            GenericError o__ = (GenericError)other__;
            if(reason == null)
            {
                if(o__.reason != null)
                {
                    return false;
                }
            }
            else
            {
                if(!reason.Equals(o__.reason))
                {
                    return false;
                }
            }
            return true;
        }

        #endregion

        #region Comparison members

        public static bool operator==(GenericError lhs__, GenericError rhs__)
        {
            return Equals(lhs__, rhs__);
        }

        public static bool operator!=(GenericError lhs__, GenericError rhs__)
        {
            return !Equals(lhs__, rhs__);
        }

        #endregion

        #region Marshaling support

        public override void write__(IceInternal.BasicStream os__)
        {
            os__.writeString("::Filesystem::GenericError");
            os__.startWriteSlice();
            os__.writeString(reason);
            os__.endWriteSlice();
        }

        public override void read__(IceInternal.BasicStream is__, bool rid__)
        {
            if(rid__)
            {
                /* string myId = */ is__.readString();
            }
            is__.startReadSlice();
            reason = is__.readString();
            is__.endReadSlice();
        }

        public override void write__(Ice.OutputStream outS__)
        {
            Ice.MarshalException ex = new Ice.MarshalException();
            ex.reason = "exception Filesystem::GenericError was not generated with stream support";
            throw ex;
        }

        public override void read__(Ice.InputStream inS__, bool rid__)
        {
            Ice.MarshalException ex = new Ice.MarshalException();
            ex.reason = "exception Filesystem::GenericError was not generated with stream support";
            throw ex;
        }

        public override bool usesClasses__()
        {
            return true;
        }

        #endregion
    }

    public interface File : Ice.Object, FileOperations_, FileOperationsNC_, Filesystem.Node
    {
    }

    public interface Directory : Ice.Object, DirectoryOperations_, DirectoryOperationsNC_, Filesystem.Node
    {
    }
}

namespace Filesystem
{
    public interface NodePrx : Ice.ObjectPrx
    {
        string name();
        string name(_System.Collections.Generic.Dictionary<string, string> context__);
    }

    public interface FilePrx : Filesystem.NodePrx
    {
        string[] read();
        string[] read(_System.Collections.Generic.Dictionary<string, string> context__);

        void write(string[] text);
        void write(string[] text, _System.Collections.Generic.Dictionary<string, string> context__);
    }

    public interface DirectoryPrx : Filesystem.NodePrx
    {
        Filesystem.NodePrx[] list();
        Filesystem.NodePrx[] list(_System.Collections.Generic.Dictionary<string, string> context__);
    }
}

namespace Filesystem
{
    public interface NodeOperations_
    {
        string name(Ice.Current current__);
    }

    public interface NodeOperationsNC_
    {
        string name();
    }

    public interface FileOperations_ : Filesystem.NodeOperations_
    {
        string[] read(Ice.Current current__);

        void write(string[] text, Ice.Current current__);
    }

    public interface FileOperationsNC_ : Filesystem.NodeOperationsNC_
    {
        string[] read();

        void write(string[] text);
    }

    public interface DirectoryOperations_ : Filesystem.NodeOperations_
    {
        Filesystem.NodePrx[] list(Ice.Current current__);
    }

    public interface DirectoryOperationsNC_ : Filesystem.NodeOperationsNC_
    {
        Filesystem.NodePrx[] list();
    }
}

namespace Filesystem
{
    public sealed class NodePrxHelper : Ice.ObjectPrxHelperBase, NodePrx
    {
        #region Synchronous operations

        public string name()
        {
            return name(null, false);
        }

        public string name(_System.Collections.Generic.Dictionary<string, string> context__)
        {
            return name(context__, true);
        }

        private string name(_System.Collections.Generic.Dictionary<string, string> context__, bool explicitContext__)
        {
            if(explicitContext__ && context__ == null)
            {
                context__ = emptyContext_;
            }
            int cnt__ = 0;
            while(true)
            {
                Ice.ObjectDel_ delBase__ = null;
                try
                {
                    checkTwowayOnly__("name");
                    delBase__ = getDelegate__(false);
                    NodeDel_ del__ = (NodeDel_)delBase__;
                    return del__.name(context__);
                }
                catch(IceInternal.LocalExceptionWrapper ex__)
                {
                    handleExceptionWrapperRelaxed__(delBase__, ex__, null, ref cnt__);
                }
                catch(Ice.LocalException ex__)
                {
                    handleException__(delBase__, ex__, null, ref cnt__);
                }
            }
        }

        #endregion

        #region Checked and unchecked cast operations

        public static NodePrx checkedCast(Ice.ObjectPrx b)
        {
            if(b == null)
            {
                return null;
            }
            NodePrx r = b as NodePrx;
            if((r == null) && b.ice_isA("::Filesystem::Node"))
            {
                NodePrxHelper h = new NodePrxHelper();
                h.copyFrom__(b);
                r = h;
            }
            return r;
        }

        public static NodePrx checkedCast(Ice.ObjectPrx b, _System.Collections.Generic.Dictionary<string, string> ctx)
        {
            if(b == null)
            {
                return null;
            }
            NodePrx r = b as NodePrx;
            if((r == null) && b.ice_isA("::Filesystem::Node", ctx))
            {
                NodePrxHelper h = new NodePrxHelper();
                h.copyFrom__(b);
                r = h;
            }
            return r;
        }

        public static NodePrx checkedCast(Ice.ObjectPrx b, string f)
        {
            if(b == null)
            {
                return null;
            }
            Ice.ObjectPrx bb = b.ice_facet(f);
            try
            {
                if(bb.ice_isA("::Filesystem::Node"))
                {
                    NodePrxHelper h = new NodePrxHelper();
                    h.copyFrom__(bb);
                    return h;
                }
            }
            catch(Ice.FacetNotExistException)
            {
            }
            return null;
        }

        public static NodePrx checkedCast(Ice.ObjectPrx b, string f, _System.Collections.Generic.Dictionary<string, string> ctx)
        {
            if(b == null)
            {
                return null;
            }
            Ice.ObjectPrx bb = b.ice_facet(f);
            try
            {
                if(bb.ice_isA("::Filesystem::Node", ctx))
                {
                    NodePrxHelper h = new NodePrxHelper();
                    h.copyFrom__(bb);
                    return h;
                }
            }
            catch(Ice.FacetNotExistException)
            {
            }
            return null;
        }

        public static NodePrx uncheckedCast(Ice.ObjectPrx b)
        {
            if(b == null)
            {
                return null;
            }
            NodePrx r = b as NodePrx;
            if(r == null)
            {
                NodePrxHelper h = new NodePrxHelper();
                h.copyFrom__(b);
                r = h;
            }
            return r;
        }

        public static NodePrx uncheckedCast(Ice.ObjectPrx b, string f)
        {
            if(b == null)
            {
                return null;
            }
            Ice.ObjectPrx bb = b.ice_facet(f);
            NodePrxHelper h = new NodePrxHelper();
            h.copyFrom__(bb);
            return h;
        }

        #endregion

        #region Marshaling support

        protected override Ice.ObjectDelM_ createDelegateM__()
        {
            return new NodeDelM_();
        }

        protected override Ice.ObjectDelD_ createDelegateD__()
        {
            return new NodeDelD_();
        }

        public static void write__(IceInternal.BasicStream os__, NodePrx v__)
        {
            os__.writeProxy(v__);
        }

        public static NodePrx read__(IceInternal.BasicStream is__)
        {
            Ice.ObjectPrx proxy = is__.readProxy();
            if(proxy != null)
            {
                NodePrxHelper result = new NodePrxHelper();
                result.copyFrom__(proxy);
                return result;
            }
            return null;
        }

        #endregion
    }

    public sealed class LinesHelper
    {
        public static void write(IceInternal.BasicStream os__, string[] v__)
        {
            os__.writeStringSeq(v__);
        }

        public static string[] read(IceInternal.BasicStream is__)
        {
            string[] v__;
            v__ = is__.readStringSeq();
            return v__;
        }
    }

    public sealed class FilePrxHelper : Ice.ObjectPrxHelperBase, FilePrx
    {
        #region Synchronous operations

        public string[] read()
        {
            return read(null, false);
        }

        public string[] read(_System.Collections.Generic.Dictionary<string, string> context__)
        {
            return read(context__, true);
        }

        private string[] read(_System.Collections.Generic.Dictionary<string, string> context__, bool explicitContext__)
        {
            if(explicitContext__ && context__ == null)
            {
                context__ = emptyContext_;
            }
            int cnt__ = 0;
            while(true)
            {
                Ice.ObjectDel_ delBase__ = null;
                try
                {
                    checkTwowayOnly__("read");
                    delBase__ = getDelegate__(false);
                    FileDel_ del__ = (FileDel_)delBase__;
                    return del__.read(context__);
                }
                catch(IceInternal.LocalExceptionWrapper ex__)
                {
                    handleExceptionWrapperRelaxed__(delBase__, ex__, null, ref cnt__);
                }
                catch(Ice.LocalException ex__)
                {
                    handleException__(delBase__, ex__, null, ref cnt__);
                }
            }
        }

        public void write(string[] text)
        {
            write(text, null, false);
        }

        public void write(string[] text, _System.Collections.Generic.Dictionary<string, string> context__)
        {
            write(text, context__, true);
        }

        private void write(string[] text, _System.Collections.Generic.Dictionary<string, string> context__, bool explicitContext__)
        {
            if(explicitContext__ && context__ == null)
            {
                context__ = emptyContext_;
            }
            int cnt__ = 0;
            while(true)
            {
                Ice.ObjectDel_ delBase__ = null;
                try
                {
                    checkTwowayOnly__("write");
                    delBase__ = getDelegate__(false);
                    FileDel_ del__ = (FileDel_)delBase__;
                    del__.write(text, context__);
                    return;
                }
                catch(IceInternal.LocalExceptionWrapper ex__)
                {
                    handleExceptionWrapperRelaxed__(delBase__, ex__, null, ref cnt__);
                }
                catch(Ice.LocalException ex__)
                {
                    handleException__(delBase__, ex__, null, ref cnt__);
                }
            }
        }

        public string name()
        {
            return name(null, false);
        }

        public string name(_System.Collections.Generic.Dictionary<string, string> context__)
        {
            return name(context__, true);
        }

        private string name(_System.Collections.Generic.Dictionary<string, string> context__, bool explicitContext__)
        {
            if(explicitContext__ && context__ == null)
            {
                context__ = emptyContext_;
            }
            int cnt__ = 0;
            while(true)
            {
                Ice.ObjectDel_ delBase__ = null;
                try
                {
                    checkTwowayOnly__("name");
                    delBase__ = getDelegate__(false);
                    FileDel_ del__ = (FileDel_)delBase__;
                    return del__.name(context__);
                }
                catch(IceInternal.LocalExceptionWrapper ex__)
                {
                    handleExceptionWrapperRelaxed__(delBase__, ex__, null, ref cnt__);
                }
                catch(Ice.LocalException ex__)
                {
                    handleException__(delBase__, ex__, null, ref cnt__);
                }
            }
        }

        #endregion

        #region Checked and unchecked cast operations

        public static FilePrx checkedCast(Ice.ObjectPrx b)
        {
            if(b == null)
            {
                return null;
            }
            FilePrx r = b as FilePrx;
            if((r == null) && b.ice_isA("::Filesystem::File"))
            {
                FilePrxHelper h = new FilePrxHelper();
                h.copyFrom__(b);
                r = h;
            }
            return r;
        }

        public static FilePrx checkedCast(Ice.ObjectPrx b, _System.Collections.Generic.Dictionary<string, string> ctx)
        {
            if(b == null)
            {
                return null;
            }
            FilePrx r = b as FilePrx;
            if((r == null) && b.ice_isA("::Filesystem::File", ctx))
            {
                FilePrxHelper h = new FilePrxHelper();
                h.copyFrom__(b);
                r = h;
            }
            return r;
        }

        public static FilePrx checkedCast(Ice.ObjectPrx b, string f)
        {
            if(b == null)
            {
                return null;
            }
            Ice.ObjectPrx bb = b.ice_facet(f);
            try
            {
                if(bb.ice_isA("::Filesystem::File"))
                {
                    FilePrxHelper h = new FilePrxHelper();
                    h.copyFrom__(bb);
                    return h;
                }
            }
            catch(Ice.FacetNotExistException)
            {
            }
            return null;
        }

        public static FilePrx checkedCast(Ice.ObjectPrx b, string f, _System.Collections.Generic.Dictionary<string, string> ctx)
        {
            if(b == null)
            {
                return null;
            }
            Ice.ObjectPrx bb = b.ice_facet(f);
            try
            {
                if(bb.ice_isA("::Filesystem::File", ctx))
                {
                    FilePrxHelper h = new FilePrxHelper();
                    h.copyFrom__(bb);
                    return h;
                }
            }
            catch(Ice.FacetNotExistException)
            {
            }
            return null;
        }

        public static FilePrx uncheckedCast(Ice.ObjectPrx b)
        {
            if(b == null)
            {
                return null;
            }
            FilePrx r = b as FilePrx;
            if(r == null)
            {
                FilePrxHelper h = new FilePrxHelper();
                h.copyFrom__(b);
                r = h;
            }
            return r;
        }

        public static FilePrx uncheckedCast(Ice.ObjectPrx b, string f)
        {
            if(b == null)
            {
                return null;
            }
            Ice.ObjectPrx bb = b.ice_facet(f);
            FilePrxHelper h = new FilePrxHelper();
            h.copyFrom__(bb);
            return h;
        }

        #endregion

        #region Marshaling support

        protected override Ice.ObjectDelM_ createDelegateM__()
        {
            return new FileDelM_();
        }

        protected override Ice.ObjectDelD_ createDelegateD__()
        {
            return new FileDelD_();
        }

        public static void write__(IceInternal.BasicStream os__, FilePrx v__)
        {
            os__.writeProxy(v__);
        }

        public static FilePrx read__(IceInternal.BasicStream is__)
        {
            Ice.ObjectPrx proxy = is__.readProxy();
            if(proxy != null)
            {
                FilePrxHelper result = new FilePrxHelper();
                result.copyFrom__(proxy);
                return result;
            }
            return null;
        }

        #endregion
    }

    public sealed class NodeSeqHelper
    {
        public static void write(IceInternal.BasicStream os__, Filesystem.NodePrx[] v__)
        {
            if(v__ == null)
            {
                os__.writeSize(0);
            }
            else
            {
                os__.writeSize(v__.Length);
                for(int ix__ = 0; ix__ < v__.Length; ++ix__)
                {
                    Filesystem.NodePrxHelper.write__(os__, v__[ix__]);
                }
            }
        }

        public static Filesystem.NodePrx[] read(IceInternal.BasicStream is__)
        {
            Filesystem.NodePrx[] v__;
            {
                int szx__ = is__.readSize();
                is__.startSeq(szx__, 2);
                v__ = new Filesystem.NodePrx[szx__];
                for(int ix__ = 0; ix__ < szx__; ++ix__)
                {
                    v__[ix__] = Filesystem.NodePrxHelper.read__(is__);
                    is__.checkSeq();
                    is__.endElement();
                }
                is__.endSeq(szx__);
            }
            return v__;
        }
    }

    public sealed class DirectoryPrxHelper : Ice.ObjectPrxHelperBase, DirectoryPrx
    {
        #region Synchronous operations

        public Filesystem.NodePrx[] list()
        {
            return list(null, false);
        }

        public Filesystem.NodePrx[] list(_System.Collections.Generic.Dictionary<string, string> context__)
        {
            return list(context__, true);
        }

        private Filesystem.NodePrx[] list(_System.Collections.Generic.Dictionary<string, string> context__, bool explicitContext__)
        {
            if(explicitContext__ && context__ == null)
            {
                context__ = emptyContext_;
            }
            int cnt__ = 0;
            while(true)
            {
                Ice.ObjectDel_ delBase__ = null;
                try
                {
                    checkTwowayOnly__("list");
                    delBase__ = getDelegate__(false);
                    DirectoryDel_ del__ = (DirectoryDel_)delBase__;
                    return del__.list(context__);
                }
                catch(IceInternal.LocalExceptionWrapper ex__)
                {
                    handleExceptionWrapperRelaxed__(delBase__, ex__, null, ref cnt__);
                }
                catch(Ice.LocalException ex__)
                {
                    handleException__(delBase__, ex__, null, ref cnt__);
                }
            }
        }

        public string name()
        {
            return name(null, false);
        }

        public string name(_System.Collections.Generic.Dictionary<string, string> context__)
        {
            return name(context__, true);
        }

        private string name(_System.Collections.Generic.Dictionary<string, string> context__, bool explicitContext__)
        {
            if(explicitContext__ && context__ == null)
            {
                context__ = emptyContext_;
            }
            int cnt__ = 0;
            while(true)
            {
                Ice.ObjectDel_ delBase__ = null;
                try
                {
                    checkTwowayOnly__("name");
                    delBase__ = getDelegate__(false);
                    DirectoryDel_ del__ = (DirectoryDel_)delBase__;
                    return del__.name(context__);
                }
                catch(IceInternal.LocalExceptionWrapper ex__)
                {
                    handleExceptionWrapperRelaxed__(delBase__, ex__, null, ref cnt__);
                }
                catch(Ice.LocalException ex__)
                {
                    handleException__(delBase__, ex__, null, ref cnt__);
                }
            }
        }

        #endregion

        #region Checked and unchecked cast operations

        public static DirectoryPrx checkedCast(Ice.ObjectPrx b)
        {
            if(b == null)
            {
                return null;
            }
            DirectoryPrx r = b as DirectoryPrx;
            if((r == null) && b.ice_isA("::Filesystem::Directory"))
            {
                DirectoryPrxHelper h = new DirectoryPrxHelper();
                h.copyFrom__(b);
                r = h;
            }
            return r;
        }

        public static DirectoryPrx checkedCast(Ice.ObjectPrx b, _System.Collections.Generic.Dictionary<string, string> ctx)
        {
            if(b == null)
            {
                return null;
            }
            DirectoryPrx r = b as DirectoryPrx;
            if((r == null) && b.ice_isA("::Filesystem::Directory", ctx))
            {
                DirectoryPrxHelper h = new DirectoryPrxHelper();
                h.copyFrom__(b);
                r = h;
            }
            return r;
        }

        public static DirectoryPrx checkedCast(Ice.ObjectPrx b, string f)
        {
            if(b == null)
            {
                return null;
            }
            Ice.ObjectPrx bb = b.ice_facet(f);
            try
            {
                if(bb.ice_isA("::Filesystem::Directory"))
                {
                    DirectoryPrxHelper h = new DirectoryPrxHelper();
                    h.copyFrom__(bb);
                    return h;
                }
            }
            catch(Ice.FacetNotExistException)
            {
            }
            return null;
        }

        public static DirectoryPrx checkedCast(Ice.ObjectPrx b, string f, _System.Collections.Generic.Dictionary<string, string> ctx)
        {
            if(b == null)
            {
                return null;
            }
            Ice.ObjectPrx bb = b.ice_facet(f);
            try
            {
                if(bb.ice_isA("::Filesystem::Directory", ctx))
                {
                    DirectoryPrxHelper h = new DirectoryPrxHelper();
                    h.copyFrom__(bb);
                    return h;
                }
            }
            catch(Ice.FacetNotExistException)
            {
            }
            return null;
        }

        public static DirectoryPrx uncheckedCast(Ice.ObjectPrx b)
        {
            if(b == null)
            {
                return null;
            }
            DirectoryPrx r = b as DirectoryPrx;
            if(r == null)
            {
                DirectoryPrxHelper h = new DirectoryPrxHelper();
                h.copyFrom__(b);
                r = h;
            }
            return r;
        }

        public static DirectoryPrx uncheckedCast(Ice.ObjectPrx b, string f)
        {
            if(b == null)
            {
                return null;
            }
            Ice.ObjectPrx bb = b.ice_facet(f);
            DirectoryPrxHelper h = new DirectoryPrxHelper();
            h.copyFrom__(bb);
            return h;
        }

        #endregion

        #region Marshaling support

        protected override Ice.ObjectDelM_ createDelegateM__()
        {
            return new DirectoryDelM_();
        }

        protected override Ice.ObjectDelD_ createDelegateD__()
        {
            return new DirectoryDelD_();
        }

        public static void write__(IceInternal.BasicStream os__, DirectoryPrx v__)
        {
            os__.writeProxy(v__);
        }

        public static DirectoryPrx read__(IceInternal.BasicStream is__)
        {
            Ice.ObjectPrx proxy = is__.readProxy();
            if(proxy != null)
            {
                DirectoryPrxHelper result = new DirectoryPrxHelper();
                result.copyFrom__(proxy);
                return result;
            }
            return null;
        }

        #endregion
    }
}

namespace Filesystem
{
    public interface NodeDel_ : Ice.ObjectDel_
    {
        string name(_System.Collections.Generic.Dictionary<string, string> context__);
    }

    public interface FileDel_ : Filesystem.NodeDel_
    {
        string[] read(_System.Collections.Generic.Dictionary<string, string> context__);

        void write(string[] text, _System.Collections.Generic.Dictionary<string, string> context__);
    }

    public interface DirectoryDel_ : Filesystem.NodeDel_
    {
        Filesystem.NodePrx[] list(_System.Collections.Generic.Dictionary<string, string> context__);
    }
}

namespace Filesystem
{
    public sealed class NodeDelM_ : Ice.ObjectDelM_, NodeDel_
    {
        public string name(_System.Collections.Generic.Dictionary<string, string> context__)
        {
            IceInternal.Outgoing og__ = handler__.getOutgoing("name", Ice.OperationMode.Idempotent, context__);
            try
            {
                bool ok__ = og__.invoke();
                try
                {
                    if(!ok__)
                    {
                        try
                        {
                            og__.throwUserException();
                        }
                        catch(Ice.UserException ex)
                        {
                            throw new Ice.UnknownUserException(ex.ice_name(), ex);
                        }
                    }
                    IceInternal.BasicStream is__ = og__.istr();
                    is__.startReadEncaps();
                    string ret__;
                    ret__ = is__.readString();
                    is__.endReadEncaps();
                    return ret__;
                }
                catch(Ice.LocalException ex__)
                {
                    throw new IceInternal.LocalExceptionWrapper(ex__, false);
                }
            }
            finally
            {
                handler__.reclaimOutgoing(og__);
            }
        }
    }

    public sealed class FileDelM_ : Ice.ObjectDelM_, FileDel_
    {
        public string[] read(_System.Collections.Generic.Dictionary<string, string> context__)
        {
            IceInternal.Outgoing og__ = handler__.getOutgoing("read", Ice.OperationMode.Idempotent, context__);
            try
            {
                bool ok__ = og__.invoke();
                try
                {
                    if(!ok__)
                    {
                        try
                        {
                            og__.throwUserException();
                        }
                        catch(Ice.UserException ex)
                        {
                            throw new Ice.UnknownUserException(ex.ice_name(), ex);
                        }
                    }
                    IceInternal.BasicStream is__ = og__.istr();
                    is__.startReadEncaps();
                    string[] ret__;
                    ret__ = is__.readStringSeq();
                    is__.endReadEncaps();
                    return ret__;
                }
                catch(Ice.LocalException ex__)
                {
                    throw new IceInternal.LocalExceptionWrapper(ex__, false);
                }
            }
            finally
            {
                handler__.reclaimOutgoing(og__);
            }
        }

        public void write(string[] text, _System.Collections.Generic.Dictionary<string, string> context__)
        {
            IceInternal.Outgoing og__ = handler__.getOutgoing("write", Ice.OperationMode.Idempotent, context__);
            try
            {
                try
                {
                    IceInternal.BasicStream os__ = og__.ostr();
                    os__.writeStringSeq(text);
                }
                catch(Ice.LocalException ex__)
                {
                    og__.abort(ex__);
                }
                bool ok__ = og__.invoke();
                try
                {
                    if(!ok__)
                    {
                        try
                        {
                            og__.throwUserException();
                        }
                        catch(Filesystem.GenericError)
                        {
                            throw;
                        }
                        catch(Ice.UserException ex)
                        {
                            throw new Ice.UnknownUserException(ex.ice_name(), ex);
                        }
                    }
                    IceInternal.BasicStream is__ = og__.istr();
                    is__.startReadEncaps();
                    is__.endReadEncaps();
                }
                catch(Ice.LocalException ex__)
                {
                    throw new IceInternal.LocalExceptionWrapper(ex__, false);
                }
            }
            finally
            {
                handler__.reclaimOutgoing(og__);
            }
        }

        public string name(_System.Collections.Generic.Dictionary<string, string> context__)
        {
            IceInternal.Outgoing og__ = handler__.getOutgoing("name", Ice.OperationMode.Idempotent, context__);
            try
            {
                bool ok__ = og__.invoke();
                try
                {
                    if(!ok__)
                    {
                        try
                        {
                            og__.throwUserException();
                        }
                        catch(Ice.UserException ex)
                        {
                            throw new Ice.UnknownUserException(ex.ice_name(), ex);
                        }
                    }
                    IceInternal.BasicStream is__ = og__.istr();
                    is__.startReadEncaps();
                    string ret__;
                    ret__ = is__.readString();
                    is__.endReadEncaps();
                    return ret__;
                }
                catch(Ice.LocalException ex__)
                {
                    throw new IceInternal.LocalExceptionWrapper(ex__, false);
                }
            }
            finally
            {
                handler__.reclaimOutgoing(og__);
            }
        }
    }

    public sealed class DirectoryDelM_ : Ice.ObjectDelM_, DirectoryDel_
    {
        public Filesystem.NodePrx[] list(_System.Collections.Generic.Dictionary<string, string> context__)
        {
            IceInternal.Outgoing og__ = handler__.getOutgoing("list", Ice.OperationMode.Idempotent, context__);
            try
            {
                bool ok__ = og__.invoke();
                try
                {
                    if(!ok__)
                    {
                        try
                        {
                            og__.throwUserException();
                        }
                        catch(Ice.UserException ex)
                        {
                            throw new Ice.UnknownUserException(ex.ice_name(), ex);
                        }
                    }
                    IceInternal.BasicStream is__ = og__.istr();
                    is__.startReadEncaps();
                    Filesystem.NodePrx[] ret__;
                    {
                        int szx__ = is__.readSize();
                        is__.startSeq(szx__, 2);
                        ret__ = new Filesystem.NodePrx[szx__];
                        for(int ix__ = 0; ix__ < szx__; ++ix__)
                        {
                            ret__[ix__] = Filesystem.NodePrxHelper.read__(is__);
                            is__.checkSeq();
                            is__.endElement();
                        }
                        is__.endSeq(szx__);
                    }
                    is__.endReadEncaps();
                    return ret__;
                }
                catch(Ice.LocalException ex__)
                {
                    throw new IceInternal.LocalExceptionWrapper(ex__, false);
                }
            }
            finally
            {
                handler__.reclaimOutgoing(og__);
            }
        }

        public string name(_System.Collections.Generic.Dictionary<string, string> context__)
        {
            IceInternal.Outgoing og__ = handler__.getOutgoing("name", Ice.OperationMode.Idempotent, context__);
            try
            {
                bool ok__ = og__.invoke();
                try
                {
                    if(!ok__)
                    {
                        try
                        {
                            og__.throwUserException();
                        }
                        catch(Ice.UserException ex)
                        {
                            throw new Ice.UnknownUserException(ex.ice_name(), ex);
                        }
                    }
                    IceInternal.BasicStream is__ = og__.istr();
                    is__.startReadEncaps();
                    string ret__;
                    ret__ = is__.readString();
                    is__.endReadEncaps();
                    return ret__;
                }
                catch(Ice.LocalException ex__)
                {
                    throw new IceInternal.LocalExceptionWrapper(ex__, false);
                }
            }
            finally
            {
                handler__.reclaimOutgoing(og__);
            }
        }
    }
}

namespace Filesystem
{
    public sealed class NodeDelD_ : Ice.ObjectDelD_, NodeDel_
    {
        public string name(_System.Collections.Generic.Dictionary<string, string> context__)
        {
            Ice.Current current__ = new Ice.Current();
            initCurrent__(ref current__, "name", Ice.OperationMode.Idempotent, context__);
            string result__ = null;
            IceInternal.Direct.RunDelegate run__ = delegate(Ice.Object obj__)
            {
                Node servant__ = null;
                try
                {
                    servant__ = (Node)obj__;
                }
                catch(_System.InvalidCastException)
                {
                    throw new Ice.OperationNotExistException(current__.id, current__.facet, current__.operation);
                }
                result__ = servant__.name(current__);
                return Ice.DispatchStatus.DispatchOK;
            };
            IceInternal.Direct direct__ = null;
            try
            {
                direct__ = new IceInternal.Direct(current__, run__);
                try
                {
                    Ice.DispatchStatus status__ = direct__.servant().collocDispatch__(direct__);
                    _System.Diagnostics.Debug.Assert(status__ == Ice.DispatchStatus.DispatchOK);
                }
                finally
                {
                    direct__.destroy();
                }
            }
            catch(Ice.SystemException)
            {
                throw;
            }
            catch(System.Exception ex__)
            {
                IceInternal.LocalExceptionWrapper.throwWrapper(ex__);
            }
            return result__;
        }
    }

    public sealed class FileDelD_ : Ice.ObjectDelD_, FileDel_
    {
        public string[] read(_System.Collections.Generic.Dictionary<string, string> context__)
        {
            Ice.Current current__ = new Ice.Current();
            initCurrent__(ref current__, "read", Ice.OperationMode.Idempotent, context__);
            string[] result__ = null;
            IceInternal.Direct.RunDelegate run__ = delegate(Ice.Object obj__)
            {
                File servant__ = null;
                try
                {
                    servant__ = (File)obj__;
                }
                catch(_System.InvalidCastException)
                {
                    throw new Ice.OperationNotExistException(current__.id, current__.facet, current__.operation);
                }
                result__ = servant__.read(current__);
                return Ice.DispatchStatus.DispatchOK;
            };
            IceInternal.Direct direct__ = null;
            try
            {
                direct__ = new IceInternal.Direct(current__, run__);
                try
                {
                    Ice.DispatchStatus status__ = direct__.servant().collocDispatch__(direct__);
                    _System.Diagnostics.Debug.Assert(status__ == Ice.DispatchStatus.DispatchOK);
                }
                finally
                {
                    direct__.destroy();
                }
            }
            catch(Ice.SystemException)
            {
                throw;
            }
            catch(System.Exception ex__)
            {
                IceInternal.LocalExceptionWrapper.throwWrapper(ex__);
            }
            return result__;
        }

        public void write(string[] text, _System.Collections.Generic.Dictionary<string, string> context__)
        {
            Ice.Current current__ = new Ice.Current();
            initCurrent__(ref current__, "write", Ice.OperationMode.Idempotent, context__);
            Ice.UserException userException__ = null;
            IceInternal.Direct.RunDelegate run__ = delegate(Ice.Object obj__)
            {
                File servant__ = null;
                try
                {
                    servant__ = (File)obj__;
                }
                catch(_System.InvalidCastException)
                {
                    throw new Ice.OperationNotExistException(current__.id, current__.facet, current__.operation);
                }
                try
                {
                    servant__.write(text, current__);
                    return Ice.DispatchStatus.DispatchOK;
                }
                catch(Ice.UserException ex__)
                {
                    userException__ = ex__;
                    return Ice.DispatchStatus.DispatchUserException;
                }
            };
            IceInternal.Direct direct__ = null;
            try
            {
                direct__ = new IceInternal.Direct(current__, run__);
                try
                {
                    Ice.DispatchStatus status__ = direct__.servant().collocDispatch__(direct__);
                    if(status__ == Ice.DispatchStatus.DispatchUserException)
                    {
                        throw userException__;
                    }
                    _System.Diagnostics.Debug.Assert(status__ == Ice.DispatchStatus.DispatchOK);
                }
                finally
                {
                    direct__.destroy();
                }
            }
            catch(Filesystem.GenericError)
            {
                throw;
            }
            catch(Ice.SystemException)
            {
                throw;
            }
            catch(System.Exception ex__)
            {
                IceInternal.LocalExceptionWrapper.throwWrapper(ex__);
            }
        }

        public string name(_System.Collections.Generic.Dictionary<string, string> context__)
        {
            Ice.Current current__ = new Ice.Current();
            initCurrent__(ref current__, "name", Ice.OperationMode.Idempotent, context__);
            string result__ = null;
            IceInternal.Direct.RunDelegate run__ = delegate(Ice.Object obj__)
            {
                File servant__ = null;
                try
                {
                    servant__ = (File)obj__;
                }
                catch(_System.InvalidCastException)
                {
                    throw new Ice.OperationNotExistException(current__.id, current__.facet, current__.operation);
                }
                result__ = servant__.name(current__);
                return Ice.DispatchStatus.DispatchOK;
            };
            IceInternal.Direct direct__ = null;
            try
            {
                direct__ = new IceInternal.Direct(current__, run__);
                try
                {
                    Ice.DispatchStatus status__ = direct__.servant().collocDispatch__(direct__);
                    _System.Diagnostics.Debug.Assert(status__ == Ice.DispatchStatus.DispatchOK);
                }
                finally
                {
                    direct__.destroy();
                }
            }
            catch(Ice.SystemException)
            {
                throw;
            }
            catch(System.Exception ex__)
            {
                IceInternal.LocalExceptionWrapper.throwWrapper(ex__);
            }
            return result__;
        }
    }

    public sealed class DirectoryDelD_ : Ice.ObjectDelD_, DirectoryDel_
    {
        public Filesystem.NodePrx[] list(_System.Collections.Generic.Dictionary<string, string> context__)
        {
            Ice.Current current__ = new Ice.Current();
            initCurrent__(ref current__, "list", Ice.OperationMode.Idempotent, context__);
            Filesystem.NodePrx[] result__ = null;
            IceInternal.Direct.RunDelegate run__ = delegate(Ice.Object obj__)
            {
                Directory servant__ = null;
                try
                {
                    servant__ = (Directory)obj__;
                }
                catch(_System.InvalidCastException)
                {
                    throw new Ice.OperationNotExistException(current__.id, current__.facet, current__.operation);
                }
                result__ = servant__.list(current__);
                return Ice.DispatchStatus.DispatchOK;
            };
            IceInternal.Direct direct__ = null;
            try
            {
                direct__ = new IceInternal.Direct(current__, run__);
                try
                {
                    Ice.DispatchStatus status__ = direct__.servant().collocDispatch__(direct__);
                    _System.Diagnostics.Debug.Assert(status__ == Ice.DispatchStatus.DispatchOK);
                }
                finally
                {
                    direct__.destroy();
                }
            }
            catch(Ice.SystemException)
            {
                throw;
            }
            catch(System.Exception ex__)
            {
                IceInternal.LocalExceptionWrapper.throwWrapper(ex__);
            }
            return result__;
        }

        public string name(_System.Collections.Generic.Dictionary<string, string> context__)
        {
            Ice.Current current__ = new Ice.Current();
            initCurrent__(ref current__, "name", Ice.OperationMode.Idempotent, context__);
            string result__ = null;
            IceInternal.Direct.RunDelegate run__ = delegate(Ice.Object obj__)
            {
                Directory servant__ = null;
                try
                {
                    servant__ = (Directory)obj__;
                }
                catch(_System.InvalidCastException)
                {
                    throw new Ice.OperationNotExistException(current__.id, current__.facet, current__.operation);
                }
                result__ = servant__.name(current__);
                return Ice.DispatchStatus.DispatchOK;
            };
            IceInternal.Direct direct__ = null;
            try
            {
                direct__ = new IceInternal.Direct(current__, run__);
                try
                {
                    Ice.DispatchStatus status__ = direct__.servant().collocDispatch__(direct__);
                    _System.Diagnostics.Debug.Assert(status__ == Ice.DispatchStatus.DispatchOK);
                }
                finally
                {
                    direct__.destroy();
                }
            }
            catch(Ice.SystemException)
            {
                throw;
            }
            catch(System.Exception ex__)
            {
                IceInternal.LocalExceptionWrapper.throwWrapper(ex__);
            }
            return result__;
        }
    }
}

namespace Filesystem
{
    public abstract class NodeDisp_ : Ice.ObjectImpl, Node
    {
        #region Slice operations

        public string name()
        {
            return name(Ice.ObjectImpl.defaultCurrent);
        }

        public abstract string name(Ice.Current current__);

        #endregion

        #region Slice type-related members

        public static new string[] ids__ = 
        {
            "::Filesystem::Node",
            "::Ice::Object"
        };

        public override bool ice_isA(string s)
        {
            return _System.Array.BinarySearch(ids__, s, IceUtilInternal.StringUtil.OrdinalStringComparer) >= 0;
        }

        public override bool ice_isA(string s, Ice.Current current__)
        {
            return _System.Array.BinarySearch(ids__, s, IceUtilInternal.StringUtil.OrdinalStringComparer) >= 0;
        }

        public override string[] ice_ids()
        {
            return ids__;
        }

        public override string[] ice_ids(Ice.Current current__)
        {
            return ids__;
        }

        public override string ice_id()
        {
            return ids__[0];
        }

        public override string ice_id(Ice.Current current__)
        {
            return ids__[0];
        }

        public static new string ice_staticId()
        {
            return ids__[0];
        }

        #endregion

        #region Operation dispatch

        public static Ice.DispatchStatus name___(Node obj__, IceInternal.Incoming inS__, Ice.Current current__)
        {
            checkMode__(Ice.OperationMode.Idempotent, current__.mode);
            inS__.istr().skipEmptyEncaps();
            IceInternal.BasicStream os__ = inS__.ostr();
            string ret__ = obj__.name(current__);
            os__.writeString(ret__);
            return Ice.DispatchStatus.DispatchOK;
        }

        private static string[] all__ =
        {
            "ice_id",
            "ice_ids",
            "ice_isA",
            "ice_ping",
            "name"
        };

        public override Ice.DispatchStatus dispatch__(IceInternal.Incoming inS__, Ice.Current current__)
        {
            int pos = _System.Array.BinarySearch(all__, current__.operation, IceUtilInternal.StringUtil.OrdinalStringComparer);
            if(pos < 0)
            {
                throw new Ice.OperationNotExistException(current__.id, current__.facet, current__.operation);
            }

            switch(pos)
            {
                case 0:
                {
                    return ice_id___(this, inS__, current__);
                }
                case 1:
                {
                    return ice_ids___(this, inS__, current__);
                }
                case 2:
                {
                    return ice_isA___(this, inS__, current__);
                }
                case 3:
                {
                    return ice_ping___(this, inS__, current__);
                }
                case 4:
                {
                    return name___(this, inS__, current__);
                }
            }

            _System.Diagnostics.Debug.Assert(false);
            throw new Ice.OperationNotExistException(current__.id, current__.facet, current__.operation);
        }

        #endregion

        #region Marshaling support

        public override void write__(IceInternal.BasicStream os__)
        {
            os__.writeTypeId(ice_staticId());
            os__.startWriteSlice();
            os__.endWriteSlice();
            base.write__(os__);
        }

        public override void read__(IceInternal.BasicStream is__, bool rid__)
        {
            if(rid__)
            {
                /* string myId = */ is__.readTypeId();
            }
            is__.startReadSlice();
            is__.endReadSlice();
            base.read__(is__, true);
        }

        public override void write__(Ice.OutputStream outS__)
        {
            Ice.MarshalException ex = new Ice.MarshalException();
            ex.reason = "type Filesystem::Node was not generated with stream support";
            throw ex;
        }

        public override void read__(Ice.InputStream inS__, bool rid__)
        {
            Ice.MarshalException ex = new Ice.MarshalException();
            ex.reason = "type Filesystem::Node was not generated with stream support";
            throw ex;
        }

        #endregion
    }

    public abstract class FileDisp_ : Ice.ObjectImpl, File
    {
        #region Slice operations

        public string[] read()
        {
            return read(Ice.ObjectImpl.defaultCurrent);
        }

        public abstract string[] read(Ice.Current current__);

        public void write(string[] text)
        {
            write(text, Ice.ObjectImpl.defaultCurrent);
        }

        public abstract void write(string[] text, Ice.Current current__);

        #endregion

        #region Inherited Slice operations

        public string name()
        {
            return name(Ice.ObjectImpl.defaultCurrent);
        }

        public abstract string name(Ice.Current current__);

        #endregion

        #region Slice type-related members

        public static new string[] ids__ = 
        {
            "::Filesystem::File",
            "::Filesystem::Node",
            "::Ice::Object"
        };

        public override bool ice_isA(string s)
        {
            return _System.Array.BinarySearch(ids__, s, IceUtilInternal.StringUtil.OrdinalStringComparer) >= 0;
        }

        public override bool ice_isA(string s, Ice.Current current__)
        {
            return _System.Array.BinarySearch(ids__, s, IceUtilInternal.StringUtil.OrdinalStringComparer) >= 0;
        }

        public override string[] ice_ids()
        {
            return ids__;
        }

        public override string[] ice_ids(Ice.Current current__)
        {
            return ids__;
        }

        public override string ice_id()
        {
            return ids__[0];
        }

        public override string ice_id(Ice.Current current__)
        {
            return ids__[0];
        }

        public static new string ice_staticId()
        {
            return ids__[0];
        }

        #endregion

        #region Operation dispatch

        public static Ice.DispatchStatus read___(File obj__, IceInternal.Incoming inS__, Ice.Current current__)
        {
            checkMode__(Ice.OperationMode.Idempotent, current__.mode);
            inS__.istr().skipEmptyEncaps();
            IceInternal.BasicStream os__ = inS__.ostr();
            string[] ret__ = obj__.read(current__);
            os__.writeStringSeq(ret__);
            return Ice.DispatchStatus.DispatchOK;
        }

        public static Ice.DispatchStatus write___(File obj__, IceInternal.Incoming inS__, Ice.Current current__)
        {
            checkMode__(Ice.OperationMode.Idempotent, current__.mode);
            IceInternal.BasicStream is__ = inS__.istr();
            is__.startReadEncaps();
            string[] text;
            text = is__.readStringSeq();
            is__.endReadEncaps();
            IceInternal.BasicStream os__ = inS__.ostr();
            try
            {
                obj__.write(text, current__);
                return Ice.DispatchStatus.DispatchOK;
            }
            catch(Filesystem.GenericError ex)
            {
                os__.writeUserException(ex);
                return Ice.DispatchStatus.DispatchUserException;
            }
        }

        private static string[] all__ =
        {
            "ice_id",
            "ice_ids",
            "ice_isA",
            "ice_ping",
            "name",
            "read",
            "write"
        };

        public override Ice.DispatchStatus dispatch__(IceInternal.Incoming inS__, Ice.Current current__)
        {
            int pos = _System.Array.BinarySearch(all__, current__.operation, IceUtilInternal.StringUtil.OrdinalStringComparer);
            if(pos < 0)
            {
                throw new Ice.OperationNotExistException(current__.id, current__.facet, current__.operation);
            }

            switch(pos)
            {
                case 0:
                {
                    return ice_id___(this, inS__, current__);
                }
                case 1:
                {
                    return ice_ids___(this, inS__, current__);
                }
                case 2:
                {
                    return ice_isA___(this, inS__, current__);
                }
                case 3:
                {
                    return ice_ping___(this, inS__, current__);
                }
                case 4:
                {
                    return Filesystem.NodeDisp_.name___(this, inS__, current__);
                }
                case 5:
                {
                    return read___(this, inS__, current__);
                }
                case 6:
                {
                    return write___(this, inS__, current__);
                }
            }

            _System.Diagnostics.Debug.Assert(false);
            throw new Ice.OperationNotExistException(current__.id, current__.facet, current__.operation);
        }

        #endregion

        #region Marshaling support

        public override void write__(IceInternal.BasicStream os__)
        {
            os__.writeTypeId(ice_staticId());
            os__.startWriteSlice();
            os__.endWriteSlice();
            base.write__(os__);
        }

        public override void read__(IceInternal.BasicStream is__, bool rid__)
        {
            if(rid__)
            {
                /* string myId = */ is__.readTypeId();
            }
            is__.startReadSlice();
            is__.endReadSlice();
            base.read__(is__, true);
        }

        public override void write__(Ice.OutputStream outS__)
        {
            Ice.MarshalException ex = new Ice.MarshalException();
            ex.reason = "type Filesystem::File was not generated with stream support";
            throw ex;
        }

        public override void read__(Ice.InputStream inS__, bool rid__)
        {
            Ice.MarshalException ex = new Ice.MarshalException();
            ex.reason = "type Filesystem::File was not generated with stream support";
            throw ex;
        }

        #endregion
    }

    public abstract class DirectoryDisp_ : Ice.ObjectImpl, Directory
    {
        #region Slice operations

        public Filesystem.NodePrx[] list()
        {
            return list(Ice.ObjectImpl.defaultCurrent);
        }

        public abstract Filesystem.NodePrx[] list(Ice.Current current__);

        #endregion

        #region Inherited Slice operations

        public string name()
        {
            return name(Ice.ObjectImpl.defaultCurrent);
        }

        public abstract string name(Ice.Current current__);

        #endregion

        #region Slice type-related members

        public static new string[] ids__ = 
        {
            "::Filesystem::Directory",
            "::Filesystem::Node",
            "::Ice::Object"
        };

        public override bool ice_isA(string s)
        {
            return _System.Array.BinarySearch(ids__, s, IceUtilInternal.StringUtil.OrdinalStringComparer) >= 0;
        }

        public override bool ice_isA(string s, Ice.Current current__)
        {
            return _System.Array.BinarySearch(ids__, s, IceUtilInternal.StringUtil.OrdinalStringComparer) >= 0;
        }

        public override string[] ice_ids()
        {
            return ids__;
        }

        public override string[] ice_ids(Ice.Current current__)
        {
            return ids__;
        }

        public override string ice_id()
        {
            return ids__[0];
        }

        public override string ice_id(Ice.Current current__)
        {
            return ids__[0];
        }

        public static new string ice_staticId()
        {
            return ids__[0];
        }

        #endregion

        #region Operation dispatch

        public static Ice.DispatchStatus list___(Directory obj__, IceInternal.Incoming inS__, Ice.Current current__)
        {
            checkMode__(Ice.OperationMode.Idempotent, current__.mode);
            inS__.istr().skipEmptyEncaps();
            IceInternal.BasicStream os__ = inS__.ostr();
            Filesystem.NodePrx[] ret__ = obj__.list(current__);
            if(ret__ == null)
            {
                os__.writeSize(0);
            }
            else
            {
                os__.writeSize(ret__.Length);
                for(int ix__ = 0; ix__ < ret__.Length; ++ix__)
                {
                    Filesystem.NodePrxHelper.write__(os__, ret__[ix__]);
                }
            }
            return Ice.DispatchStatus.DispatchOK;
        }

        private static string[] all__ =
        {
            "ice_id",
            "ice_ids",
            "ice_isA",
            "ice_ping",
            "list",
            "name"
        };

        public override Ice.DispatchStatus dispatch__(IceInternal.Incoming inS__, Ice.Current current__)
        {
            int pos = _System.Array.BinarySearch(all__, current__.operation, IceUtilInternal.StringUtil.OrdinalStringComparer);
            if(pos < 0)
            {
                throw new Ice.OperationNotExistException(current__.id, current__.facet, current__.operation);
            }

            switch(pos)
            {
                case 0:
                {
                    return ice_id___(this, inS__, current__);
                }
                case 1:
                {
                    return ice_ids___(this, inS__, current__);
                }
                case 2:
                {
                    return ice_isA___(this, inS__, current__);
                }
                case 3:
                {
                    return ice_ping___(this, inS__, current__);
                }
                case 4:
                {
                    return list___(this, inS__, current__);
                }
                case 5:
                {
                    return Filesystem.NodeDisp_.name___(this, inS__, current__);
                }
            }

            _System.Diagnostics.Debug.Assert(false);
            throw new Ice.OperationNotExistException(current__.id, current__.facet, current__.operation);
        }

        #endregion

        #region Marshaling support

        public override void write__(IceInternal.BasicStream os__)
        {
            os__.writeTypeId(ice_staticId());
            os__.startWriteSlice();
            os__.endWriteSlice();
            base.write__(os__);
        }

        public override void read__(IceInternal.BasicStream is__, bool rid__)
        {
            if(rid__)
            {
                /* string myId = */ is__.readTypeId();
            }
            is__.startReadSlice();
            is__.endReadSlice();
            base.read__(is__, true);
        }

        public override void write__(Ice.OutputStream outS__)
        {
            Ice.MarshalException ex = new Ice.MarshalException();
            ex.reason = "type Filesystem::Directory was not generated with stream support";
            throw ex;
        }

        public override void read__(Ice.InputStream inS__, bool rid__)
        {
            Ice.MarshalException ex = new Ice.MarshalException();
            ex.reason = "type Filesystem::Directory was not generated with stream support";
            throw ex;
        }

        #endregion
    }
}
