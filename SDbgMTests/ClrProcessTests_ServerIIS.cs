﻿using System;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using SDbgCore;
using SDbgM;
//using SDbgM;

namespace SDbgMTests
{
    [TestClass]
    public class ClrProcessTests_ServerIIS
    {
        private MSDbgExt _proc;

        [TestInitialize]
        public void Init()
        {
            _proc = new MSDbgExt(@"Q:\Dev\SDbgExt2\Dumps\x86\iis_small.dmp");
        }

        [TestMethod]
        public void GetStaticFieldValues_IISSmall()
        {
            var type = _proc.Process.FindTypeByName("System.Web.dll", "System.Web.HttpRuntime");
            ulong field;
            ClrFieldDescData fd;
            _proc.Process.FindFieldByNameEx(type, "_theRuntime", out field, out fd);
            
            var values = _proc.GetStaticFieldValues(field);

            //Assert.AreEqual(2, values.Length);
            //Assert.AreEqual(0, values[0].domain);
            //hr = p->FindFieldByNameEx(typeMT, L"_field2", &field, &fdData);
            //hr = p->GetStaticFieldValues(field, 1, &values, &iValues);
        }
    }
}
