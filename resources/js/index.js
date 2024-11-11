const display_div = document.querySelector("#display_div");
const display_list = document.querySelector("#display_list");

function searchHotels(needle)
{
    let url = "/api/hotel/search?needle=";
    if (typeof needle !== 'undefined')
        url += needle;   
    
    fetch(url)
        .then((response) => { if (!response.ok) {
                throw new Error(`HTTP error: ${response.status}`); }
            return response.json();
        }).then((json) => {
            for (let i = 0, len = json.length; i < len; i++) {
                let e = document.createElement("li");
                let a = document.createElement("a");
                
                let d = document.createElement("button");
                d.type = "button";
                d.innerHTML = "delete";
                d.param1 = json[i].id;
                d.addEventListener("click", deleteHotel.bind(d));
                
                a.setAttribute("href", "/api/hotel?id=" + json[i].id);
                a.innerHTML = json[i].name + " hotel";
                e.appendChild(a);
                e.appendChild(d);
                display_list.appendChild(e);
            }
        })
        .catch((error) => {
            display_list.textContent = `Could not fetch verse: ${error}`;
        });

}

function deleteHotel(e)
{
    let url = "/api/hotel?id=";
    if (typeof e.currentTarget.param1 !== 'undefined')
        url += e.currentTarget.param1;
    
    fetch(url, {method: "DELETE"})
        .then((response) => { 
            
            if (!response.ok) {
                throw new Error(`HTTP error: ${response.status}`); 
            }

            while (display_list.firstChild) {
                display_list.removeChild(display_list.lastChild);
            }
            
            searchHotels("");
            
            return response.json();})
    
        .catch((error) => {
            return `Could not fetch verse: ${error}`;
        });
}



























