using System;
using System.Collections.Generic;

namespace DesignPatterns
{
	public interface INamedClass {
		string ClassName { get; set; }
	}
	public class Factory<TFactory> where TFactory : class, INamedClass
	{
		public delegate TFactory Builder (params string[] arguments);
		public Factory ()
		{
		}
		public void Register<TClass> (Builder creator) where TClass : TFactory
		{
			TFactory fake = creator ();
			Console.WriteLine ("Registering: " + fake.ClassName);
			creators [fake.ClassName] = creator;
		}
		public TFactory Create ( string name, params string[] arguments )
		{
			if (creators.ContainsKey (name))
				return creators [name] (arguments);
			return null;
		}
		//
		public Dictionary<string,Builder> creators = new Dictionary<string, Builder> ();
	}
}

