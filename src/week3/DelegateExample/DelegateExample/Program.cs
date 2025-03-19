using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ConsoleApplication1
{
    class Event
    {
    }

    class ScoreBasketEvent : Event
    {
        public int Points { get; set; }
        public ScoreBasketEvent(int points)
        {
            Points = points;
        }
    }

    class EventManager
    {
        public delegate void EventHandler(Event evt);
        public event EventHandler ScoreBasketEvent;

        public void Run()
        {
            while (true)
            {
                System.Threading.Thread.Sleep(1500);
                if (ScoreBasketEvent != null)
                {
                    ScoreBasketEvent(new ScoreBasketEvent(2));
                }
            }
        }
    }

    class HUD
    {
        public void RegisterForEvents(EventManager evtManager)
        {
            evtManager.ScoreBasketEvent += this.HandleScoreBasketEvent;
        }

        private void HandleScoreBasketEvent(Event p_pEvent)
        {
            ScoreBasketEvent sbe = p_pEvent as ScoreBasketEvent;
            Console.WriteLine("HandleScoreBasketEvent: Handling basket worth {0} points", sbe.Points);
        }
    }

    class Program
    {
        static void Main(string[] args)
        {
            EventManager eventManager = new EventManager();
            HUD hud = new HUD();

            hud.RegisterForEvents(eventManager);
            eventManager.Run();
        }
    }
}
