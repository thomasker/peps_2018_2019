using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using System.Runtime.InteropServices;
using Microsoft.AspNetCore.Mvc;

namespace prisma.Controllers
{
    
    [Route("api/[controller]")]
    public class ValuesController : Controller
    {
        [DllImport(@"cpp/build/libprisma-cpp.so")]
        public static extern void sayHelloWorld();

        [DllImport(@"cpp/build/libprisma-cpp.so")]
        public static extern int returnInteger();
        
        // GET api/values
        [HttpGet]
        public IEnumerable<string> Get()
        {
            string value2 = String.Concat("value", returnInteger());
            return new string[] { "value1", value2 };
        }

        // GET api/values/5
        [HttpGet("{id}")]
        public string Get(int id)
        {
            return String.Concat("value", id);
        }

        // POST api/values
        [HttpPost]
        public void Post([FromBody]string value)
        {
            sayHelloWorld();
        }

        // PUT api/values/5
        [HttpPut("{id}")]
        public void Put(int id, [FromBody]string value)
        {
        }

        // DELETE api/values/5
        [HttpDelete("{id}")]
        public void Delete(int id)
        {
        }
    }
}
