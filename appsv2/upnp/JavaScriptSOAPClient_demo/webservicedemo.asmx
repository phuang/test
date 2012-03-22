<%@ WebService Language="C#" Class="WebServiceDemo" %>

using System;
using System.Web;
using System.Web.Services;
using System.Web.Services.Protocols;

[WebService(Namespace = "http://www.guru4.net/", Description = "Webservice dimostrativo per JavaScript SOAP Client")]
public class WebServiceDemo : WebService 
{

    [WebMethod(Description = "Hello World!")]
    public string HelloWorld() 
	{
        return "Hello World!";
    }

	[WebMethod(Description = "Hello [YOUR NAME HERE]")]
	public string HelloTo(string name)
	{
		return "Hello " + name + "!";
	}

	[WebMethod(Description = "Restituisce la date e l'ora correnti")]
	public DateTime ServerTime()
	{
		return DateTime.Now;
	}

	[WebMethod(Description = "Il metodo resta in attesa per <i>n</i> secondi")]
	public void Wait(int seconds)
	{
		System.Threading.Thread.Sleep(seconds * 1000);
		return;
	}

	[WebMethod(Description = "Il metodo solleva un'eccezione")]
	public void ThrowException(int seconds)
	{
		throw new Exception();
	}

	[WebMethod(Description = "Restituisce un'istanza di User (custom object dimostrativo)")]
	public User GetUser(string username)
	{
		if (username.Trim().Length == 0)
			return null;
		int id = DateTime.Now.Millisecond;
		string password = "PWD_" + DateTime.Now.Ticks.ToString();
		DateTime expirationdate = DateTime.Now.Add(new TimeSpan(1, 0, 0, 0));
		return new User(id, username, password, expirationdate);
	}

	[WebMethod(Description = "Restituisce un array di User (array di custom object dimostrativi)")]
	public User[] GetUsers()
	{
		User[] ul = new User[4];
		Random r = new Random();
		for (int i = 0; i < ul.Length; i++)
		{
			int id = r.Next(100);
			string username = "USR_" + id.ToString();
			string password = "PWD_" + id.ToString();
			DateTime expirationdate = DateTime.Now.Add(new TimeSpan((i + 1), 0, 0, 0));
			ul[i] = new User(id, username, password, expirationdate);
		}
		return ul;
	}

	[WebMethod(Description = "Restituisce una collection di User (UserList, collection dimostrativa di User)")]
	public UserList GetUserList()
	{
		UserList ul = new UserList();
		Random r = new Random();
		for (int i = 0; i < 3; i++)
		{
			int id = r.Next(100);
			string username = "USR_" + id.ToString();
			string password = "PWD_" + id.ToString();
			DateTime expirationdate = DateTime.Now.Add(new TimeSpan((i + 1), 0, 0, 0));
			ul.Add(new User(id, username, password, expirationdate));
		}
		return ul;
	}

	[WebMethod(Description = "Restituisce un array di Car (array di custom object dimostrativi) sulla base del produttore selezionato")]
	public Car[] GetCars(string companyid)
	{
		Car[] cl;
		switch (companyid.Trim().ToLower())
		{
			case "vw":
				cl = new Car[] 
				{
					new Car(1, "Passat"),
					new Car(2, "Golf"),
					new Car(3, "Polo"),
					new Car(4, "Lupo")
				};
				break;
			case "f":
				cl = new Car[] 
				{
					new Car(1, "Stilo"),
					new Car(2, "Punto"),
					new Car(3, "500")
				};
				break;
			case "bmw":
				cl = new Car[] 
				{
					new Car(1, "X5"),
					new Car(2, "520")
				};
				break;
			default:
				cl = new Car[0];
				break;
		}
		return cl;
	}

}

[Serializable]
public class User
{
	private int _id = -1;
	private string _username = "";
	private string _password = "";
	private DateTime _expirationdate = DateTime.MinValue;
	public User() { }
	public User(int id, string username, string password, DateTime expirationdate)
	{
		this.Id = id;
		this.Username = username;
		this.Password = password;
		this.ExpirationDate = expirationdate;
	}
	public int Id
	{
		get { return _id; }
		set { _id = value; }
	}
	public string Username
	{
		get { return _username; }
		set { _username = value; }
	}
	public string Password
	{
		get { return _password; }
		set { _password = value; }
	}
	public DateTime ExpirationDate
	{
		get { return _expirationdate; }
		set { _expirationdate = value; }
	}
}
[Serializable]
public class UserList : System.Collections.CollectionBase
{
	public UserList() { }
	public int Add(User value)
	{
		return base.List.Add(value as object);
	}
	public User this[int index]
	{
		get { return (base.List[index] as User); }
	}
	public void Remove(User value)
	{
		base.List.Remove(value as object);
	}
}

[Serializable]
public class Car
{
	private int _id = -1;
	private string _label = "";
	public Car() { }
	public Car(int id, string label)
	{
		this.Id = id;
		this.Label = label;
	}
	public int Id
	{
		get { return _id; }
		set { _id = value; }
	}
	public string Label
	{
		get { return _label; }
		set { _label = value; }
	}
}

