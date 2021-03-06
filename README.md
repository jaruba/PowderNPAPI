# Powder NPAPI Plugin

**Deprecated: Since December 2015 there are no longer any browsers that support NPAPI plugins, this repo will remain up in hopes that it might still have some value for someone in the future.**

Peerflix interface for the browser. NPAPI Plugin made with FireBreath + Node.js + Peerflix.

[**SEE OUTPUT DEMO**](http://jaruba.github.io/PowderNPAPI/)

[**SEE STREAMING DEMO**](http://jaruba.github.io/PowderNPAPI/webchimera-demo/) (also requires [WebChimera Plugin](http://www.webchimera.org/download) installed and enabled on the page)

**Embedding in page:**

    <object id="powder" type="application/x-powdernpapi" width="1" height="1"></object>

**Streaming Demo Source Files:** [gh-pages branch](https://github.com/jaruba/PowderNPAPI/tree/gh-pages/webchimera-demo)

JS API
==============

**Start Peerflix** - requires the info hash from a magnet link, returns a stream id (int). The streaming link will be returned in the "torrentData" event.

    .getStream(infoHash)

**Request Data** - requires a stream id, returns "true" if new data is available and "false" if no new data is available. This function does not return the data in order to not freeze the page while waiting for a response, the data will come when the "torrentData" event is triggered.

    .requestData(streamId)

**Read Data** - event that returns a stream id and json data from peerflix. The JSON Object can contain: runtime, path, uploaded, downloaded, peerqueue, seeds, link, filename, size.

    torrentData(streamId,jsonData)

**Kill Stream** - terminates a process by stream id.

    .killStream(streamId)

**Kill All** - terminates all peerflix processes.

    .killAll()
