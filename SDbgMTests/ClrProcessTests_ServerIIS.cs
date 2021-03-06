﻿using System;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using SDbgCore;
using SPT.Managed;

namespace SDbgMTests
{
    [TestClass]
    public class ClrProcessTests_ServerIIS
    {
        private SptWrapper _proc;

        [TestInitialize]
        public void Init()
        {
            _proc = Util.CreateFromDump(@"Q:\spt\Dumps\x86\iis_request.dmp");
        }

        [TestMethod]
        public void GetStaticFieldValues_Managed()
        {
            var type = _proc.Proc.FindTypeByName("System.Web.dll", "System.Web.HttpRuntime");
            var field = _proc.Proc.FindFieldByName(type, "_theRuntime");
            var values = _proc.GetStaticFieldValues(field);

            Assert.AreEqual(2, values.Length);
            Assert.AreEqual((ulong)0x0000000013fa6300, values[0].Value);
        }

        [TestMethod]
        public void GetFieldValueString_Managed()
        {
            //11fa8ca8 
            var str = _proc.GetFieldValueString(0x11fa8da0, "_pathTranslated");
            Assert.AreEqual(@"Q:\Dev\SOSRevHelper\IISHelper\default.aspx", str);
        }
    }
}
